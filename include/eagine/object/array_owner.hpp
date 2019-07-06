/**
 *  @file eagine/object/array_owner.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EAGINE_OBJECT_ARRAY_OWNER_HPP
#define EAGINE_OBJECT_ARRAY_OWNER_HPP

#include "names.hpp"
#include "operations.hpp"
#include "owner.hpp"

namespace eagine {

template <typename ObjTag, std::size_t N>
class object_array_owner : public owned<object_name_array<ObjTag, N>> {
private:
    const object_name_array<ObjTag, N>& _ona() const {
        return *this;
    }

public:
    object_array_owner()
      : owned<object_name_array<ObjTag, N>>() {
        obj_lifetime_ops<ObjTag>::gen_objects(*this);
    }

    object_array_owner(object_subtype_t<ObjTag> subtype)
      : owned<object_name_array<ObjTag, N>>() {
        obj_lifetime_ops<ObjTag>::gen_objects(*this, subtype);
    }

    object_array_owner(object_array_owner&&) noexcept = default;
    object_array_owner(const object_array_owner&) = delete;
    object_array_owner& operator=(object_array_owner&&) noexcept = default;
    object_array_owner& operator=(const object_array_owner&) = delete;

    ~object_array_owner() noexcept {
        try {
            obj_lifetime_ops<ObjTag>::delete_objects(*this);
        } catch(...) {
        }
    }

    object_name_and_ops<ObjTag> operator[](span_size_t i) const noexcept {
        return _ona()[i];
    }
};

template <typename ObjTag>
class object_vector_owner : public owned<object_name_vector<ObjTag>> {
private:
    const object_name_vector<ObjTag>& _ona() const {
        return *this;
    }

public:
    object_vector_owner(span_size_t n)
      : owned<object_name_vector<ObjTag>>(n) {
        obj_lifetime_ops<ObjTag>::gen_objects(*this);
    }

    object_vector_owner(span_size_t n, object_subtype_t<ObjTag> subtype)
      : owned<object_name_vector<ObjTag>>(n) {
        obj_lifetime_ops<ObjTag>::gen_objects(*this, subtype);
    }

    object_vector_owner(object_vector_owner&&) noexcept = default;
    object_vector_owner(const object_vector_owner&) = delete;
    object_vector_owner& operator=(object_vector_owner&&) noexcept = default;
    object_vector_owner& operator=(const object_vector_owner&) = delete;

    ~object_vector_owner() noexcept {
        try {
            obj_lifetime_ops<ObjTag>::delete_objects(*this);
        } catch(...) {
        } // TODO rethrow exceptions or cancel ?
    }

    object_name_and_ops<ObjTag> operator[](span_size_t i) const noexcept {
        return _ona()[i];
    }
};

} // namespace eagine

#endif // EAGINE_OBJECT_ARRAY_OWNER_HPP
