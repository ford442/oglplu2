/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_VALUE_TREE_WRAPPERS_HPP
#define EAGINE_VALUE_TREE_WRAPPERS_HPP

#include "../assert.hpp"
#include "../callable_ref.hpp"
#include "../memory/span_algo.hpp"
#include "../reflect/enumerators.hpp"
#include "../type_identity.hpp"
#include "../valid_if/decl.hpp"
#include "interface.hpp"
#include <utility>

namespace eagine::valtree {
//------------------------------------------------------------------------------
class compound;

/// @brief Handle class for value tree attributes.
/// @ingroup valtree
/// @see compound
/// @see compound_attribute
///
/// Attributes act as key that allow access values and nested attributes
/// of some structured tree data hierarchy (JSON, YAML, XML, filesystem
/// hierarchy, etc.). An attribute represents a single node in such hierarchy
/// and can be used to retrieve the values or data stored at that level in the
/// tree hierarchy and enumerate or find nested nodes.
class attribute {
public:
    /// @brief Default constructor. Constructs empty attribute refering to nothing.
    attribute() noexcept = default;

    /// @brief Move constructor.
    attribute(attribute&& temp) noexcept
      : _owner{std::move(temp._owner)}
      , _pimpl{temp._pimpl} {
        temp._pimpl = nullptr;
    }

    /// @brief Copy constructor. Handles attribute reference counting.
    attribute(const attribute& that)
      : _owner{that._owner}
      , _pimpl{that._pimpl} {
        if(_owner && _pimpl) {
            _owner->add_ref(*_pimpl);
        }
    }

    /// @brief Move assignment operator.
    auto operator=(attribute&& temp) noexcept -> attribute& {
        if(this != std::addressof(temp)) {
            using std::swap;
            swap(_owner, temp._owner);
            swap(_pimpl, temp._pimpl);
        }
        return *this;
    }

    /// @brief Copy assignment operator. Handles attribute reference counting.
    auto operator=(const attribute& that) -> attribute& {
        if(this != std::addressof(that)) {
            using std::swap;
            attribute temp{std::move(*this)};
            _owner = that._owner;
            _pimpl = that._pimpl;
            if(_owner && _pimpl) {
                _owner->add_ref(*_pimpl);
            }
        }
        return *this;
    }

    /// @brief Destructor. Handles attribute reference counting.
    ~attribute() noexcept {
        if(_pimpl) {
            EAGINE_ASSERT(_owner);
            _owner->release(*_pimpl);
        }
    }

    /// @brief Indicates if this attribute actually refers to something.
    explicit operator bool() const {
        return _owner && _pimpl;
    }

    /// @brief Returns the implementation type id of this attribute.
    auto type_id() const noexcept -> identifier_t {
        if(_pimpl) {
            return _pimpl->type_id();
        }
        return 0;
    }

    /// @brief Returns the implementation type id of this attribute.
    auto name() const -> string_view {
        if(_owner && _pimpl) {
            return _owner->attribute_name(*_pimpl);
        }
        return {};
    }

private:
    friend class compound;

    attribute(
      std::shared_ptr<compound_interface> owner,
      attribute_interface* pimpl) noexcept
      : _owner{std::move(owner)}
      , _pimpl{pimpl} {}

    std::shared_ptr<compound_interface> _owner{};
    attribute_interface* _pimpl{nullptr};
};
//------------------------------------------------------------------------------
class compound_attribute;
//------------------------------------------------------------------------------
template <typename T>
struct not_converted_value {
    constexpr not_converted_value(T& dest) noexcept
      : _dest{dest} {}

    constexpr auto dest() noexcept -> auto& {
        return _dest;
    }

    template <identifier_t V>
    constexpr auto apply(selector<V>) const noexcept {
        return true;
    }

private:
    T& _dest;
};
//------------------------------------------------------------------------------
template <typename T>
struct converted_enum_value {
    static_assert(has_enumerator_mapping_v<T>);

public:
    constexpr converted_enum_value(T& dest) noexcept
      : _dest{dest} {}

    constexpr auto dest() noexcept -> auto& {
        return _temp;
    }

    template <identifier_t V>
    auto apply(selector<V> sel) const {
        if(auto converted{from_string(_temp, type_identity<T>(), sel)}) {
            _dest = extract(converted);
            return true;
        }
        return false;
    }

private:
    std::string _temp;
    T& _dest;
};
//------------------------------------------------------------------------------
template <typename T>
struct converted_value
  : std::conditional_t<
      has_enumerator_mapping_v<T>,
      converted_enum_value<T>,
      not_converted_value<T>> {

    using base = std::conditional_t<
      has_enumerator_mapping_v<T>,
      converted_enum_value<T>,
      not_converted_value<T>>;

    using base::base;
};
//------------------------------------------------------------------------------
template <>
struct converted_value<std::chrono::duration<float>>
  : not_converted_value<std::chrono::duration<float>> {
    using base = not_converted_value<std::chrono::duration<float>>;
    using base::base;
};
//------------------------------------------------------------------------------
template <typename R, typename P>
class converted_value<std::chrono::duration<R, P>> {
    using T = std::chrono::duration<R, P>;

public:
    constexpr converted_value(T& dest) noexcept
      : _dest{dest} {}

    constexpr auto dest() noexcept -> auto& {
        return _temp;
    }

    template <identifier_t V>
    auto apply(selector<V>) const {
        _dest = std::chrono::duration_cast<T>(_temp);
        return true;
    }

private:
    std::chrono::duration<float> _temp{};
    T& _dest;
};
//------------------------------------------------------------------------------
/// @brief Handle class for value tree compounds.
/// @ingroup valtree
/// @see compound
/// @see compound_attribute
///
/// Compounds own, manage and parse the actual tree data
/// of some structured tree hierarchy (JSON, YAML, XML, filesystem
/// hierarchy, etc.). A compound represents the parsed or "open" instance
/// of such a tree and allows to traverse and explore the tree structure
/// and access the stored data through an unified interface.
class compound {
public:
    /// @brief Default constructor. Constructs an empty compound.
    compound() noexcept = default;

    /// @brief Instantiates a particular implementation.
    /// @note Do not use directly in client code. Use one of the constructor
    /// functions that know which implementation to pick and how to initialize it.
    template <typename Compound, typename... Args>
    static auto make(Args&&... args) -> std::
      enable_if_t<std::is_base_of_v<compound_interface, Compound>, compound> {
        return {Compound::make_shared(std::forward<Args>(args)...)};
    }

    /// @brief Indicates if this compound actually refers to some tree.
    explicit operator bool() const noexcept {
        return bool(_pimpl);
    }

    /// @brief Returns the implementation type id of this attribute.
    auto type_id() const noexcept -> identifier_t {
        if(_pimpl) {
            return _pimpl->type_id();
        }
        return 0;
    }

    /// @brief Returns the root of attribute hierarchy describing the tree structure.
    /// @see root
    ///
    /// The returned attribute can be used to explore and traverse the tree
    /// node hierarchy by getting the names, value types and nested nodes.
    auto structure() const -> attribute {
        if(_pimpl) {
            return {_pimpl, _pimpl->structure()};
        }
        return {};
    }

    /// @brief Returns the structure root as an compound_attribute.
    /// @see structure
    auto root() const -> compound_attribute;

    /// @brief Returns the name of an attribute.
    /// @pre this->type_id() == attrib.type_id().
    auto attribute_name(const attribute& attrib) const -> string_view {
        if(_pimpl && attrib._pimpl) {
            return _pimpl->attribute_name(*attrib._pimpl);
        }
        return {};
    }

    /// @brief Returns the caninical value type of an attribute.
    /// @pre this->type_id() == attrib.type_id().
    ///
    /// Tree data referred-to by an attribute can be fetched either using
    /// the canonical type or using a different, related value type, if the
    /// necessary conversion is implemented.
    auto canonical_type(const attribute& attrib) const -> value_type {
        if(_pimpl && attrib._pimpl) {
            return _pimpl->canonical_type(*attrib._pimpl);
        }
        return value_type::unknown;
    }

    /// @brief Indicates if the specified attribute is a reference or link in the tree.
    /// @pre this->type_id() == attrib.type_id()
    auto is_link(const attribute& attrib) const -> bool {
        if(_pimpl && attrib._pimpl) {
            return _pimpl->is_link(*attrib._pimpl);
        }
        return false;
    }

    /// @brief Returns the count of nested attributes of an attribute.
    /// @pre this->type_id() == attrib.type_id()
    /// @note Some implementations may return zero here even if there are
    /// nested attributes. In such implementations the nested nodes can be
    /// traversed only by name.
    auto nested_count(const attribute& attrib) const -> span_size_t {
        if(_pimpl && attrib._pimpl) {
            return _pimpl->nested_count(*attrib._pimpl);
        }
        return 0;
    }

    /// @brief Indicates if an attribute has nested attribute accessible by index.
    /// @pre this->type_id() == attrib.type_id()
    auto has_nested(const attribute& attrib) const -> bool {
        return nested_count(attrib) != 0;
    }

    /// @brief Returns nested attribute of an attribute at the specified index.
    /// @pre this->type_id() == attrib.type_id()
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto nested(const attribute& attrib, span_size_t index) const -> attribute {
        if(_pimpl && attrib._pimpl) {
            return {_pimpl, _pimpl->nested(*attrib._pimpl, index)};
        }
        return {};
    }

    /// @brief Returns nested attribute of an attribute with the specified name.
    /// @pre this->type_id() == attrib.type_id()
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto nested(const attribute& attrib, string_view name) const -> attribute {
        if(_pimpl && attrib._pimpl) {
            return {_pimpl, _pimpl->nested(*attrib._pimpl, name)};
        }
        return {};
    }

    /// @brief Returns nested attribute of the root attribute with the specified name.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto nested(string_view name) const -> attribute {
        return nested(structure(), name);
    }

    /// @brief Returns nested attribute of an attribute at the specified path.
    /// @pre this->type_id() == attrib.type_id()
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto find(const attribute& attrib, const basic_string_path& path) const
      -> attribute {
        if(_pimpl && attrib._pimpl) {
            return {_pimpl, _pimpl->find(*attrib._pimpl, path)};
        }
        return {};
    }

    /// @brief Returns nested attribute of an attribute at path with tags.
    /// @pre this->type_id() == attrib.type_id()
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto find(
      const attribute& attrib,
      const basic_string_path& path,
      span<const string_view> tags) const -> attribute {
        if(_pimpl && attrib._pimpl) {
            return {_pimpl, _pimpl->find(*attrib._pimpl, path, tags)};
        }
        return {};
    }

    /// @brief Returns nested attribute of root attribute at the specified path.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto find(const basic_string_path& path) const -> attribute {
        return find(structure(), path);
    }

    /// @brief Returns nested attribute of root attribute at path with tags.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto find(const basic_string_path& path, span<const string_view> tags) const
      -> attribute {
        return find(structure(), path, tags);
    }

    /// @brief Returns the number of value elements accessible through an attribute.
    auto value_count(const attribute& attrib) const -> span_size_t {
        if(_pimpl && attrib._pimpl) {
            return _pimpl->value_count(*attrib._pimpl);
        }
        return 0;
    }

    /// @brief Returns the number of value elements at the specified path.
    auto value_count(const basic_string_path& path) const -> span_size_t {
        return value_count(find(path));
    }

    /// @brief Returns the number of value elements at attribute with the given name.
    auto value_count(string_view name) -> span_size_t {
        return value_count(nested(name));
    }

    /// @brief Fetches values at the given attribute, starting at offset into @p dest.
    template <typename T>
    auto fetch_values(const attribute& attrib, span_size_t offset, span<T> dest)
      const -> span<T> {
        if(_pimpl && attrib._pimpl) {
            return head(
              dest, _pimpl->fetch_values(*attrib._pimpl, offset, dest));
        }
        return {};
    }

    /// @brief Fetches values at the given path, starting at offset into @p dest.
    template <typename T>
    auto fetch_values(
      const basic_string_path& path,
      span_size_t offset,
      span<T> dest) const -> span<T> {
        return fetch_values(find(path), offset, dest);
    }

    /// @brief Fetches values at the given name, starting at offset into @p dest.
    template <typename T>
    auto fetch_values(string_view name, span_size_t offset, span<T> dest) const
      -> span<T> {
        return fetch_values(nested(name), offset, dest);
    }

    /// @brief Fetches values at the given attribute, into @p dest.
    template <typename T>
    auto fetch_values(const attribute& attrib, span<T> dest) const -> span<T> {
        return fetch_values(attrib, 0, dest);
    }

    /// @brief Fetches values at the attribute with the specified path, into @p dest.
    template <typename T>
    auto fetch_values(const basic_string_path& path, span<T> dest) const
      -> span<T> {
        return fetch_values(path, 0, dest);
    }

    /// @brief Fetches values at the attribute with the specified name, into @p dest.
    template <typename T>
    auto fetch_values(string_view name, span<T> dest) const -> span<T> {
        return fetch_values(name, 0, dest);
    }

    /// @brief Fetches a BLOB at the given attribute, into @p dest.
    auto fetch_blob(const attribute& attrib, memory::block dest) const
      -> memory::block {
        return fetch_values(attrib, dest);
    }

    /// @brief Fetches a BLOB at the attribute with the specified path, into @p dest.
    auto fetch_blob(const basic_string_path& path, memory::block dest) const
      -> memory::block {
        return fetch_values(path, dest);
    }

    /// @brief Fetches a BLOB at the attribute with the specified name, into @p dest.
    auto fetch_blob(string_view name, memory::block dest) const
      -> memory::block {
        return fetch_values(name, dest);
    }

    /// @brief Fetches a single value at the specified attribute, with a selector.
    template <typename T, identifier_t V>
    auto select_value(
      const attribute& attrib,
      span_size_t offset,
      T& dest,
      selector<V> sel) const -> bool {
        converted_value<T> conv{dest};
        if(!fetch_values(attrib, offset, cover_one(conv.dest())).empty()) {
            return conv.apply(sel);
        }
        return false;
    }

    /// @brief Fetches a single value at the specified attribute, at offset into @p dest.
    template <typename T>
    auto fetch_value(const attribute& attrib, span_size_t offset, T& dest) const
      -> bool {
        return select_value(attrib, offset, dest, default_selector);
    }

    /// @brief Fetches values at the specified attribute, with a selector, into dest.
    template <typename T, identifier_t V>
    auto select_values(
      const attribute& attrib,
      span_size_t offset,
      span<T> dest,
      selector<V> sel) const -> span<T> {
        span_size_t index = 0;
        for(T& elem : dest) {
            if(!select_value(attrib, offset + index, elem, sel)) {
                return {};
            }
            ++index;
        }
        return head(dest, index);
    }

    /// @brief Fetches values through the specified name, with a selector, into dest.
    template <typename T, identifier_t V>
    auto
    select_value(string_view name, span_size_t offset, T& dest, selector<V> sel)
      const -> bool {
        return select_value(nested(name), offset, dest, sel);
    }

    /// @brief Fetches values through the specified name, into dest.
    template <typename T>
    auto fetch_value(string_view name, span_size_t offset, T& dest) const
      -> bool {
        return select_value(name, offset, dest, default_selector);
    }

    /// @brief Fetches values through the specified path, with a selector, into dest.
    template <typename T, identifier_t V>
    auto select_value(
      const basic_string_path& path,
      span_size_t offset,
      T& dest,
      selector<V> sel) const -> bool {
        return select_value(find(path), offset, dest, sel);
    }

    /// @brief Fetches values through the specified path, into dest.
    template <typename T>
    auto fetch_value(const basic_string_path& path, span_size_t offset, T& dest)
      const -> bool {
        return select_value(path, offset, dest, default_selector);
    }

    /// @brief Fetches values through the specified name, with a selector, into dest.
    template <typename T, identifier_t V>
    auto select_value(string_view name, T& dest, selector<V> sel) const
      -> bool {
        return select_value(name, 0, dest, sel);
    }

    /// @brief Fetches values through the specified name, into dest.
    template <typename T>
    auto fetch_value(string_view name, T& dest) const -> bool {
        return select_value(name, 0, dest, default_selector);
    }

    /// @brief Fetches a value at the specified attribute, with a selector, into dest.
    template <typename T, identifier_t V>
    auto select_value(const attribute& attrib, T& dest, selector<V> sel) const
      -> bool {
        return select_value(attrib, 0, dest, sel);
    }

    /// @brief Fetches values at the specified attribute, with a selector, into dest.
    template <typename T, identifier_t V>
    auto
    select_values(const attribute& attrib, span<T> dest, selector<V> sel) const
      -> span<T> {
        return select_values(attrib, 0, dest, sel);
    }

    /// @brief Fetches a single value at the specified attribute, into dest.
    template <typename T>
    auto fetch_value(const attribute& attrib, T& dest) const -> bool {
        return select_value(attrib, 0, dest, default_selector);
    }

    /// @brief Fetches a value through the specified path, with selector, into dest.
    template <typename T, identifier_t V>
    auto
    select_value(const basic_string_path& path, T& dest, selector<V> sel) const
      -> bool {
        return select_value(path, 0, dest, sel);
    }

    /// @brief Fetches a value through the specified path, into dest.
    template <typename T>
    auto fetch_value(const basic_string_path& path, T& dest) const -> bool {
        return selector_value(path, 0, dest, default_selector);
    }

    /// @brief Tests if there is an value at an attribute, that starts with @p what.
    template <std::size_t L>
    auto has_value(const attribute& attrib, const char (&what)[L]) const
      -> bool {
        char temp[L]{};
        if(fetch_values(attrib, 0, cover(temp))) {
            return starts_with(string_view(temp), string_view(what));
        }
        return false;
    }

    /// @brief Returns the value of type T at an attribute, at offset, with selector.
    template <typename T, identifier_t V>
    auto get(
      const attribute& attrib,
      span_size_t offset,
      type_identity<T>,
      selector<V> sel) const -> optionally_valid<T> {
        T temp{};
        if(select_value(attrib, offset, temp, sel)) {
            return {std::move(temp), true};
        }
        return {};
    }

    /// @brief Returns the value of type T at an attribute, at the given offset.
    template <typename T>
    auto get(
      const attribute& attrib,
      span_size_t offset,
      type_identity<T> tid = {}) const -> optionally_valid<T> {
        return get<T>(attrib, offset, tid, default_selector);
    }

    /// @brief Returns the value of type T at path, at given offset, with selector.
    template <typename T, identifier_t V>
    auto get(
      const basic_string_path& path,
      span_size_t offset,
      type_identity<T>,
      selector<V> sel) const -> optionally_valid<T> {
        T temp{};
        if(select_value(path, offset, temp, sel)) {
            return {std::move(temp), true};
        }
        return {};
    }

    /// @brief Returns the value of type T at path, at given offset.
    template <typename T>
    auto get(
      const basic_string_path& path,
      span_size_t offset,
      type_identity<T> tid = {}) const -> optionally_valid<T> {
        return get<T>(path, offset, tid, default_selector);
    }

    /// @brief Returns the value of type T at name, at given offset, with selector.
    template <typename T, identifier_t V>
    auto
    get(string_view name, span_size_t offset, type_identity<T>, selector<V> sel)
      const -> optionally_valid<T> {
        T temp{};
        if(select_value(name, offset, temp, sel)) {
            return {std::move(temp), true};
        }
        return {};
    }

    /// @brief Returns the value of type T at name, at given offset.
    template <typename T>
    auto
    get(string_view name, span_size_t offset, type_identity<T> tid = {}) const
      -> optionally_valid<T> {
        return get<T>(name, offset, tid, default_selector);
    }

    /// @brief Returns the value of type T at an attribute, with selector.
    template <typename T, identifier_t V>
    auto
    get(const attribute& attrib, type_identity<T> tid, selector<V> sel) const
      -> optionally_valid<T> {
        return get<T>(attrib, 0, tid, sel);
    }

    /// @brief Returns the value of type T at an attribute.
    template <typename T>
    auto get(const attribute& attrib, type_identity<T> tid = {}) const
      -> optionally_valid<T> {
        return get<T>(attrib, tid, default_selector);
    }

    /// @brief Returns the value of type T at path, with selector.
    template <typename T, identifier_t V>
    auto get(
      const basic_string_path& path,
      type_identity<T> tid,
      selector<V> sel) const -> optionally_valid<T> {
        return get<T>(path, 0, tid, sel);
    }

    /// @brief Returns the value of type T at path.
    template <typename T>
    auto get(const basic_string_path& path, type_identity<T> tid = {}) const
      -> optionally_valid<T> {
        return get<T>(path, tid, default_selector);
    }

    /// @brief Returns the value of type T at name, with selector.
    template <typename T, identifier_t V>
    auto get(string_view name, type_identity<T> tid, selector<V> sel) const
      -> optionally_valid<T> {
        return get<T>(name, 0, tid, sel);
    }

    /// @brief Returns the value of type T at name.
    template <typename T>
    auto get(string_view name, type_identity<T> tid = {}) const
      -> optionally_valid<T> {
        return get<T>(name, tid, default_selector);
    }

    /// @brief Type of traverse/visit handler.
    using visit_handler =
      callable_ref<bool(compound&, const attribute&, const basic_string_path&)>;

    /// @brief Traverses the tree, calls the @p visitor function on each node.
    void traverse(visit_handler visitor);

    /// @brief Type of traverse/visit handler using full attribute stack.
    using stack_visit_handler = callable_ref<bool(
      compound&,
      const attribute&,
      const basic_string_path&,
      span<const attribute>)>;

    /// @brief Traverses the tree, calls the @p visitor function on each node.
    void traverse(stack_visit_handler visitor);

private:
    compound(std::shared_ptr<compound_interface> pimpl) noexcept
      : _pimpl{std::move(pimpl)} {}

    std::shared_ptr<compound_interface> _pimpl{};
};
//------------------------------------------------------------------------------
/// @brief Combination of a reference to a tree compound and a single attribute.
/// @ingroup valtree
///
/// This class provides a more convenient value tree access interface, at the
/// cost of storage space.
class compound_attribute {
public:
    /// @brief Default constructor. Constructs empty attribute refering to nothing.
    compound_attribute() noexcept = default;

    /// @brief Construction from a compound and attribute pair.
    /// @pre c.type_id() == a.type_id()
    compound_attribute(compound c, attribute a) noexcept
      : _c{std::move(c)}
      , _a{std::move(a)} {
        EAGINE_ASSERT(_c.type_id() == _a.type_id());
    }

    /// @brief Indicates if this attribute actually refers to something.
    explicit operator bool() const noexcept {
        return _c && _a;
    }

    /// @brief Returns the shared implementation type id of the attribute and compound.
    auto type_id() const noexcept {
        return _c.type_id();
    }

    /// @brief Returns the name of this attribute.
    auto name() const noexcept -> string_view {
        return _c.attribute_name(_a);
    }

    /// @brief Indicates if the specified attribute is a reference or link in the tree.
    auto is_link() const noexcept -> bool {
        return _c.is_link(_a);
    }

    /// @brief Returns the canonical value type of this attribute.
    auto canonical_type() const -> value_type {
        return _c.canonical_type(_a);
    }

    /// @brief Returns the count of nested attributes of an attribute.
    /// @note Some implementations may return zero here even if there are
    /// nested attributes. In such implementations the nested nodes can be
    /// traversed only by name.
    auto nested_count() const -> span_size_t {
        return _c.nested_count(_a);
    }

    /// @brief Indicates if an attribute has nested attribute accessible by index.
    auto has_nested() const -> span_size_t {
        return _c.has_nested(_a);
    }

    /// @brief Returns nested attribute of an attribute at the specified index.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto nested(span_size_t index) const -> compound_attribute {
        return {_c, _c.nested(_a, index)};
    }

    /// @brief Returns nested attribute of an attribute with the specified name.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto nested(string_view name) const -> compound_attribute {
        return {_c, _c.nested(_a, name)};
    }

    /// @brief Returns nested attribute of an attribute at the specified path.
    ///
    /// Returns empty attribute handle if no such nested attribute exists.
    auto find(const basic_string_path& path) const -> compound_attribute {
        return {_c, _c.find(_a, path)};
    }

    /// @brief Returns the number of value elements accessible through an attribute.
    auto value_count() const -> span_size_t {
        return _c.value_count(_a);
    }

    /// @brief Fetches values from this attribute, starting at offset, into dest.
    template <typename T>
    auto fetch_values(span_size_t offset, span<T> dest) const {
        return _c.fetch_values(_a, offset, dest);
    }

    /// @brief Fetches values from this attribute, into dest.
    template <typename T>
    auto fetch_values(span<T> dest) const {
        return _c.fetch_values(_a, dest);
    }

    /// @brief Fetches a BLOB from this attribute, into dest.
    auto fetch_blob(memory::block dest) const {
        return _c.fetch_blob(_a, dest);
    }

    /// @brief Fetches a value from this attribute, starting at offset, with selector.
    template <typename T, identifier_t V>
    auto fetch_value(
      span_size_t offset,
      T& dest,
      selector<V> sel = default_selector) const -> bool {
        return _c.select_value(_a, offset, dest, sel);
    }

    /// @brief Fetches a value from this attribute, with selector.
    template <typename T, identifier_t V>
    auto select_value(T& dest, selector<V> sel) const -> bool {
        return _c.select_value(_a, dest, sel);
    }

    /// @brief Fetches a value from this attribute, with selector, into dest.
    template <typename T, identifier_t V>
    auto select_values(span<T> dest, selector<V> sel) const -> span<T> {
        return _c.select_values(_a, dest, sel);
    }

    /// @brief Fetches a value from this attribute, into dest.
    template <typename T>
    auto fetch_value(T& dest) const -> bool {
        return _c.select_value(_a, dest, default_selector);
    }

    /// @brief Returns a value of type T, from this attribute, at offset.
    template <typename T>
    auto get(span_size_t offset, type_identity<T> tid = {}) const {
        return _c.get(_a, offset, tid);
    }

    /// @brief Returns a value of type T, from this attribute.
    template <typename T>
    auto get(type_identity<T> tid = {}) const {
        return _c.get(_a, tid);
    }

private:
    compound _c;
    attribute _a;
};
//------------------------------------------------------------------------------
inline auto compound::root() const -> compound_attribute {
    return {*this, structure()};
}
//------------------------------------------------------------------------------
/// @brief Operator for creating a compound_attribute from compound and attribute.
/// @ingroup valtree
static inline auto operator/(compound c, attribute a) noexcept
  -> compound_attribute {
    return {std::move(c), std::move(a)};
}
//------------------------------------------------------------------------------
} // namespace eagine::valtree

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/value_tree/wrappers.inl>
#endif

#endif // EAGINE_VALUE_TREE_WRAPPERS_HPP
