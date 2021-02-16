/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_MEMORY_SPLIT_BLOCK_HPP
#define EAGINE_MEMORY_SPLIT_BLOCK_HPP

#include "block.hpp"
#include "span_algo.hpp"

namespace eagine::memory {
//------------------------------------------------------------------------------
template <bool IsConst>
class basic_split_block {
public:
    constexpr basic_split_block() noexcept = default;
    constexpr basic_split_block(basic_block<IsConst> blk) noexcept
      : _block{blk} {}

    constexpr auto split_position() const noexcept -> span_size_t {
        return _split;
    }

    constexpr auto head() const noexcept {
        return head(_block, _split);
    }

    constexpr auto tail() const noexcept {
        return skip(_block, _split);
    }

    auto advance(span_size_t amount) noexcept -> auto& {
        _split += amount;
        return *this;
    }

    auto skip_to_end() noexcept -> auto& {
        _split = _block.size();
        return *this;
    }

private:
    basic_block<IsConst> _block{};
    span_size_t _split{0};
};
//------------------------------------------------------------------------------
using split_block = basic_split_block<false>;
using const_split_block = basic_split_block<true>;
//------------------------------------------------------------------------------
} // namespace eagine::memory

#endif // EAGINE_MEMORY_SPLIT_BLOCK_HPP
