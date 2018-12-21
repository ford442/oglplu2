/**
 *  @file eagine/memory/std_alloc.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_MEMORY_STD_ALLOC_HPP
#define EAGINE_MEMORY_STD_ALLOC_HPP

#include "default_alloc.hpp"
#include "shared_alloc.hpp"
#include <memory>

namespace eagine {
namespace memory {

// std_allocator
template <typename T>
class std_allocator : public block_owner {
private:
    shared_byte_allocator _sba;

public:
    const shared_byte_allocator& _get_sba() const {
        return _sba;
    }

    using value_type = T;
    using size_type = span_size_t;

    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    template <typename U>
    struct rebind {
        using other = std_allocator<U>;
    };

    template <typename U>
    std_allocator(const std_allocator<U>& that)
      : _sba(that._get_sba()) {
    }

    std_allocator(shared_byte_allocator&& sba) noexcept
      : _sba(std::move(sba)) {
    }

    std_allocator() noexcept
      : _sba(default_byte_allocator()) {
    }

    template <typename ByteAlloc>
    ByteAlloc& as() {
        return _sba.as<ByteAlloc>();
    }

    T* address(T& r) noexcept {
        return std::allocator<T>().address(r);
    }

    const T* address(const T& r) noexcept {
        return std::allocator<T>().address(r);
    }

    size_type max_size() const noexcept {
        return _sba.max_size(alignof(T));
    }

    T* allocate(size_type n, const void* = nullptr) {
        owned_block b = _sba.allocate(span_size_of<T>(n), span_align_of<T>());

        if(!b) {
            throw std::bad_alloc();
        }

        assert(is_aligned_to(b.addr(), span_align_of<T>()));
        assert(b.size() >= span_size_of<T>(n));

        T* p = static_cast<T*>(b.addr());

        release_block(std::move(b));

        return p;
    }

    void deallocate(T* p, size_type n) {
        _sba.deallocate(
          acquire_block(as_bytes(cover(p, span_size_of<T>(n)))),
          span_align_of<T>());
    }

    friend bool operator==(
      const std_allocator& a, const std_allocator& b) noexcept {
        return (a._sba == b._sba);
    }

    friend bool operator!=(
      const std_allocator& a, const std_allocator& b) noexcept {
        return (a._sba != b._sba);
    }

    template <class U, class... A>
    static inline void construct(U* p, A&&... a) {
        ::new(static_cast<void*>(p)) U(std::forward<A>(a)...);
    }

    template <typename U>
    static inline void destroy(U* p) noexcept(noexcept(p->~U())) {
        return p->~U();
    }
};

} // namespace memory
} // namespace eagine

#endif // EAGINE_MEMORY_STD_ALLOC_HPP
