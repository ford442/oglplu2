/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_ANY_ITERATOR_HPP
#define EAGINE_ANY_ITERATOR_HPP

#include "assert.hpp"
#include "deep_copy_ptr.hpp"
#include <iterator>

namespace eagine {

// any_forward_iterator
template <typename VT, typename RT, typename PT, typename DT>
class any_forward_iterator {
private:
    struct _intf {
        _intf() = default;
        _intf(_intf&&) noexcept = default;
        _intf(const _intf&) = default;
        _intf& operator=(_intf&&) = delete;
        _intf& operator=(const _intf&) = delete;
        virtual ~_intf() = default;

        virtual std::unique_ptr<_intf> copy() = 0;
        virtual RT _deref() = 0;
        virtual void _advance() = 0;
        virtual bool _equal(_intf*) = 0;
    };

    template <typename Iter>
    struct _impl : _intf {
        Iter _i;

        _impl(Iter i)
          : _i(i) {}

        std::unique_ptr<_intf> copy() override {
            return std::unique_ptr<_intf>(new _impl(*this));
        }

        RT _deref() override {
            return *_i;
        }

        void _advance() override {
            ++_i;
        }

        bool _equal(_intf* intf) override {
            auto* that = dynamic_cast<_impl*>(intf);
            EAGINE_ASSERT(that != nullptr);
            return this->_i == that->_i;
        }
    };

    deep_copy_ptr<_intf> _pimpl;

public:
    using value_type = VT;
    using reference = RT;
    using pointer = PT;
    using difference_type = DT;

    using iterator_category = std::forward_iterator_tag;

    any_forward_iterator() = default;
    any_forward_iterator(any_forward_iterator&&) noexcept = default;
    any_forward_iterator(const any_forward_iterator&) = default;
    any_forward_iterator& operator=(const any_forward_iterator&) = default;
    any_forward_iterator& operator=(any_forward_iterator&&) noexcept = default;
    ~any_forward_iterator() noexcept = default;

    template <typename Iter>
    any_forward_iterator(Iter i)
      : _pimpl(make_deep_copy_ptr<_impl<Iter>>(i)) {}

    friend bool
    operator==(const any_forward_iterator& a, const any_forward_iterator& b) {
        return a._pimpl->_equal(b._pimpl.get());
    }

    friend bool
    operator!=(const any_forward_iterator& a, const any_forward_iterator& b) {
        return !a._pimpl->_equal(b._pimpl.get());
    }

    reference operator*() const {
        return _pimpl->_deref();
    }

    any_forward_iterator& operator++() {
        _pimpl->_advance();
        return *this;
    }

    const any_forward_iterator operator++(int) {
        any_forward_iterator i = *this;
        _pimpl->_advance();
        return i;
    }
};

template <typename T>
using any_std_forward_iterator =
  any_forward_iterator<T, const T&, const T*, std::ptrdiff_t>;

template <typename T>
using any_copying_forward_iterator =
  any_forward_iterator<T, T, const T*, std::ptrdiff_t>;

// any_forward_iterator_range
template <typename VT, typename RT, typename PT, typename DT>
class any_forward_iterator_range {
private:
    any_forward_iterator<VT, RT, PT, DT> _bgn, _end;

public:
    using iterator = any_forward_iterator<VT, RT, PT, DT>;

    any_forward_iterator_range(iterator b, iterator e)
      : _bgn(std::move(b))
      , _end(std::move(e)) {}

    template <typename Range>
    any_forward_iterator_range(const Range& range)
      : any_forward_iterator_range(range.begin(), range.end()) {}

    iterator begin() const {
        return _bgn;
    }

    iterator end() const {
        return _end;
    }
};

template <typename T>
using any_std_forward_range =
  any_forward_iterator_range<T, const T&, const T*, std::ptrdiff_t>;

template <typename T>
using any_copying_forward_range =
  any_forward_iterator_range<T, T, const T*, std::ptrdiff_t>;

} // namespace eagine

#endif // EAGINE_ANY_ITERATOR_HPP
