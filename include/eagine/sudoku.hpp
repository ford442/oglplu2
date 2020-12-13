/**
 *  @file eagine/sudoku.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EAGINE_SUDOKU_HPP
#define EAGINE_SUDOKU_HPP

#include "assert.hpp"
#include "flat_map.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <ostream>
#include <random>
#include <stack>

namespace eagine {
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_glyph;
template <unsigned S, bool Tight = false>
class basic_sudoku_board;
template <unsigned S>
class basic_sudoku_board_generator;
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_board_traits {
public:
    static constexpr const unsigned glyph_count = S * S;

    using This = basic_sudoku_board_traits;
    using generator = basic_sudoku_board_generator<S>;
    using board_type = basic_sudoku_board<S>;

    basic_sudoku_board_traits() noexcept = default;

    basic_sudoku_board_traits(
      std::array<std::string, glyph_count> glyphs,
      std::array<std::string, S> multi_glyphs,
      std::string empty_glyph) noexcept
      : _glyphs{std::move(glyphs)}
      , _multi_glyphs{std::move(multi_glyphs)}
      , _empty_glyph{std::move(empty_glyph)} {}

    auto make_diagonal() const -> board_type;
    auto make_generator() const -> generator;

    auto print(std::ostream&, const basic_sudoku_glyph<S>& glyph) const
      -> std::ostream&;

    auto print(std::ostream&, const basic_sudoku_board<S, false>& board) const
      -> std::ostream&;

    auto print(std::ostream&, const basic_sudoku_board<S, true>& board) const
      -> std::ostream&;

private:
    std::array<std::string, glyph_count> _glyphs{};
    std::array<std::string, S> _multi_glyphs{};
    std::string _empty_glyph{};
};
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_glyph {
public:
    using board_traits = basic_sudoku_board_traits<S>;
    using cell_type = std::conditional_t<
      (board_traits::glyph_count > 32U),
      std::int64_t,
      std::conditional_t<
        (board_traits::glyph_count > 16),
        std::uint32_t,
        std::conditional_t<
          (board_traits::glyph_count > 8),
          std::uint16_t,
          std::uint8_t>>>;

    static constexpr const unsigned glyph_count = board_traits::glyph_count;

    static constexpr auto to_cell_type(unsigned index) noexcept {
        return cell_type(1U << index);
    }

    constexpr basic_sudoku_glyph() noexcept = default;
    constexpr basic_sudoku_glyph(unsigned index) noexcept
      : _cel_val{to_cell_type(index)} {}

    constexpr auto is_empty() const noexcept {
        return _cel_val == 0U;
    }

    constexpr auto is_single() const noexcept {
        return _is_pot(_cel_val);
    }

    constexpr auto is_multiple() const noexcept {
        return !is_empty() && !is_single();
    }

    auto cell_value() const noexcept -> cell_type {
        return _cel_val;
    }

    auto get_index() const noexcept -> unsigned {
        unsigned result = 0U;
        while(result < glyph_count) {
            if(_cel_val == to_cell_type(result)) {
                break;
            }
            ++result;
        }
        EAGINE_ASSERT(result < glyph_count);
        return result;
    }

    constexpr auto set(unsigned index) noexcept -> auto& {
        EAGINE_ASSERT(index < glyph_count);
        _cel_val = to_cell_type(index);
        return *this;
    }

    constexpr auto add(unsigned index) noexcept -> auto& {
        EAGINE_ASSERT(index < glyph_count);
        _cel_val |= to_cell_type(index);
        return *this;
    }

    template <typename Function>
    void for_each_alternative(Function func) const noexcept {

        for(unsigned index = 0; index < glyph_count; ++index) {
            const auto mask = to_cell_type(index);
            if((_cel_val & mask) == mask) {
                func(index);
            }
        }
    }

    auto alternative_count() const noexcept -> unsigned {
        unsigned count = 0U;
        for_each_alternative([&](auto) { ++count; });
        return count;
    }

private:
    friend class basic_sudoku_board<S, false>;
    friend class basic_sudoku_board<S, true>;

    struct from_cell_value_tag {};
    constexpr basic_sudoku_glyph(cell_type cel_val, from_cell_value_tag) noexcept
      : _cel_val{cel_val} {}

    static constexpr auto _is_pot(cell_type v) noexcept {
        return (v != 0U) && ((v & (v - 1U)) == 0U);
    }

    cell_type _cel_val{0U};
};
//------------------------------------------------------------------------------
template <unsigned S, bool Tight>
class basic_sudoku_board {
public:
    using This = basic_sudoku_board;
    using board_traits = basic_sudoku_board_traits<S>;
    using glyph_type = basic_sudoku_glyph<S>;
    using cell_type = typename glyph_type::cell_type;
    using coord_type = std::array<unsigned, 4>;
    static constexpr const unsigned glyph_count = board_traits::glyph_count;

    static constexpr auto invalid_coord() noexcept -> coord_type {
        return {{S, S, S, S}};
    }

    template <typename Function>
    void for_each_coord(Function func) const noexcept {
        for(unsigned by = 0; by < S; ++by) {
            for(unsigned bx = 0; bx < S; ++bx) {
                for(unsigned cy = 0; cy < S; ++cy) {
                    for(unsigned cx = 0; cx < S; ++cx) {
                        if(!func(coord_type{{bx, by, cx, cy}})) {
                            return;
                        }
                    }
                }
            }
        }
    }

    basic_sudoku_board(const board_traits& type) noexcept
      : _type{type} {
        for_each_coord([&](const auto& coord) {
            set(coord, glyph_type());
            return true;
        });
    }

    basic_sudoku_board(const basic_sudoku_board<S, !Tight>& that) noexcept
      : _type{that._type}
      , _blocks{that._blocks} {}

    auto type() const noexcept -> auto& {
        return _type;
    }

    friend auto operator<<(std::ostream& out, const This& that)
      -> std::ostream& {
        return that._type.get().print(out, that);
    }

    auto is_possible(const coord_type& coord, glyph_type value) const noexcept {
        const auto [cbx, cby, ccx, ccy] = coord;
        const auto cel_val = value.cell_value();

        for(unsigned cy = 0; cy < S; ++cy) {
            for(unsigned cx = 0; cx < S; ++cx) {
                if((cx != ccx) || (cy != ccy)) {
                    if(_cell_val({cbx, cby, cx, cy}) == cel_val) {
                        return false;
                    }
                }
            }
        }

        for(unsigned bx = 0; bx < S; ++bx) {
            for(unsigned cx = 0; cx < S; ++cx) {
                if((bx != cbx) || (cx != ccx)) {
                    if(_cell_val({bx, cby, cx, ccy}) == cel_val) {
                        return false;
                    }
                }
            }
        }

        for(unsigned by = 0; by < S; ++by) {
            for(unsigned cy = 0; cy < S; ++cy) {
                if((by != cby) || (cy != ccy)) {
                    if(_cell_val({cbx, by, ccx, cy}) == cel_val) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    auto is_solved() noexcept {
        bool result = true;
        for_each_coord([&](const auto& coord) {
            const auto value = get(coord);
            if(!value.is_single()) {
                result = false;
                return false;
            }
            return true;
        });
        return result;
    }

    auto has_empty() noexcept {
        bool result = false;
        for_each_coord([&](const auto& coord) {
            if(get(coord).is_empty()) {
                result = true;
                return false;
            }
            return true;
        });
        return result;
    }

    auto get(const coord_type& coord) const noexcept -> glyph_type {
        return {_cell_val(coord), typename glyph_type::from_cell_value_tag{}};
    }

    auto set(const coord_type& coord, glyph_type value) noexcept -> auto& {
        _cell_ref(coord) = value.cell_value();
        return *this;
    }

    auto set_available_alternatives(const coord_type& coord) noexcept -> auto& {
        glyph_type alternatives;
        for(unsigned value = 0; value < glyph_count; ++value) {
            if(is_possible(coord, value)) {
                alternatives.add(value);
            }
        }
        return set(coord, alternatives);
    }

    auto calculate_alternatives() noexcept -> auto& {
        for_each_coord([&](const auto& coord) {
            if(!get(coord).is_single()) {
                set_available_alternatives(coord);
            }
            return true;
        });
        return *this;
    }

    auto find_unsolved() const noexcept -> coord_type {
        auto result = invalid_coord();
        auto min_alt = glyph_count + 1;
        for_each_coord([&](const auto& coord) {
            if(const auto num_alt{get(coord).alternative_count()}) {
                if((num_alt > 1) && (min_alt > num_alt)) {
                    min_alt = num_alt;
                    result = coord;
                }
            }
            return true;
        });
        return result;
    }

    template <typename Function>
    void for_each_alternative(const coord_type& coord, Function func) const {
        if(coord != invalid_coord()) {
            get(coord).for_each_alternative([&](glyph_type alt) {
                auto candidate = This(*this).set(coord, alt);
                if(candidate.is_possible(coord, alt)) {
                    if(!candidate.calculate_alternatives().has_empty()) {
                        func(candidate);
                    }
                }
            });
        }
    }

    using block_type = std::array<cell_type, glyph_count>;

    auto get_block(unsigned bx, unsigned by) const noexcept
      -> const block_type& {
        return _block(_blocks, bx, by);
    }

    auto set_block(unsigned bx, unsigned by, const block_type& block) noexcept
      -> auto& {
        _block(_blocks, bx, by) = block;
        return *this;
    }

    auto tight() const noexcept -> basic_sudoku_board<S, true> {
        return {*this};
    }

private:
    auto _cell_val(const coord_type& coord) const noexcept {
        const auto [bx, by, cx, cy] = coord;
        return _cell(_block(_blocks, bx, by), cx, cy);
    }

    auto _cell_ref(const coord_type& coord) noexcept -> auto& {
        const auto [bx, by, cx, cy] = coord;
        return _cell(_block(_blocks, bx, by), cx, cy);
    }

    template <typename B>
    static auto _cell(B& block, unsigned x, unsigned y) noexcept -> auto& {
        return block[y * S + x];
    }

    template <typename B>
    static auto _block(B& blocks, unsigned x, unsigned y) noexcept -> auto& {
        return blocks[y * S + x];
    }

    friend class basic_sudoku_board<S, !Tight>;

    std::reference_wrapper<const board_traits> _type;
    std::array<block_type, glyph_count> _blocks{};
};
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_board_generator {
public:
    using board_traits = basic_sudoku_board_traits<S>;
    using board_type = basic_sudoku_board<S>;

    basic_sudoku_board_generator(const board_traits& traits)
      : _traits{traits} {}

    auto generate(std::size_t count) -> board_type {
        board_type result{_traits};
        result.calculate_alternatives();

        while(count) {
            const typename board_type::coord_type coord{
              _coord_dist(_rd),
              _coord_dist(_rd),
              _coord_dist(_rd),
              _coord_dist(_rd)};

            if(!result.get(coord).is_single()) {
                while(true) {
                    const auto value{_glyph_dist(_rd)};
                    if(result.is_possible(coord, value)) {
                        result.set(coord, value).calculate_alternatives();
                        --count;
                        break;
                    }
                }
            }
        }

        return result;
    }

    auto generate_one() {
        return generate(1);
    }

    auto generate_few() {
        return generate(S + S);
    }

    auto generate_medium() {
        return generate(S * S + S + S);
    }

    auto generate_many() {
        return generate(S * S * S + S + S + S);
    }

private:
    std::reference_wrapper<const board_traits> _traits;
    std::random_device _rd;
    std::uniform_int_distribution<unsigned> _coord_dist{0, S - 1};
    std::uniform_int_distribution<unsigned> _glyph_dist{0, S* S - 1};
};
//------------------------------------------------------------------------------
template <unsigned S>
auto basic_sudoku_board_traits<S>::make_diagonal() const -> board_type {
    board_type result{*this};
    unsigned g = 0;
    for(unsigned b = 0; b < S; ++b) {
        for(unsigned c = 0; c < S; ++c) {
            result.set({{b, b, c, c}}, g++);
        }
    }
    return result.calculate_alternatives();
}
//------------------------------------------------------------------------------
template <unsigned S>
auto basic_sudoku_board_traits<S>::make_generator() const -> generator {
    return {*this};
}
//------------------------------------------------------------------------------
template <unsigned S>
auto basic_sudoku_board_traits<S>::print(
  std::ostream& out,
  const basic_sudoku_glyph<S>& glyph) const -> std::ostream& {
    if(glyph.is_single()) {
        out << _glyphs[glyph.get_index()];
    } else if(glyph.is_empty()) {
        out << _empty_glyph;
    } else {
        out << _multi_glyphs[0U];
    }
    return out;
}
//------------------------------------------------------------------------------
template <unsigned S>
auto basic_sudoku_board_traits<S>::print(
  std::ostream& out,
  const basic_sudoku_board<S, false>& board) const -> std::ostream& {
    for(unsigned by = 0; by < S; ++by) {
        for(unsigned cy = 0; cy < S; ++cy) {
            for(unsigned bx = 0; bx < S; ++bx) {
                for(unsigned cx = 0; cx < S; ++cx) {
                    print(out << ' ', board.get({{bx, by, cx, cy}}));
                }
                if(bx + 1 < S) {
                    out << " |";
                }
            }
            out << '\n';
        }
        if(by + 1 < S) {
            for(unsigned s1 = 0; s1 < S; ++s1) {
                for(unsigned s2 = 0; s2 < S; ++s2) {
                    if(s1 == 0 && s2 == 0) {
                        out << " -";
                    } else {
                        out << "--";
                    }
                }
                if(s1 + 1 < S) {
                    out << "-+";
                }
            }
            out << '\n';
        }
    }
    return out;
}
//------------------------------------------------------------------------------
template <unsigned S>
auto basic_sudoku_board_traits<S>::print(
  std::ostream& out,
  const basic_sudoku_board<S, true>& board) const -> std::ostream& {
    for(unsigned by = 0; by < S; ++by) {
        for(unsigned cy = 0; cy < S; ++cy) {
            for(unsigned bx = 0; bx < S; ++bx) {
                for(unsigned cx = 0; cx < S; ++cx) {
                    print(out, board.get({{bx, by, cx, cy}}));
                }
            }
            out << '\n';
        }
    }
    return out;
}
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_solver {
public:
    using board_type = basic_sudoku_board<S>;

    auto solve(board_type board) -> board_type {
        std::stack<board_type> solutions;
        solutions.push(board);

        bool done = false;
        while(!(solutions.empty() || done)) {
            board = solutions.top();
            solutions.pop();

            board.for_each_alternative(
              board.find_unsolved(), [&](auto candidate) {
                  if(candidate.is_solved()) {
                      board = candidate;
                      done = true;
                  } else {
                      solutions.push(candidate);
                  }
              });
        }
        return board;
    }
};
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_tiling;
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_tile_patch {
    static_assert(S > 2U);
    static constexpr const span_size_t M = S * (S - 2);

public:
    basic_sudoku_tile_patch(span_size_t w, span_size_t h)
      : _width{w % M ? (1 + w / M) * M : w}
      , _height{h % M ? (1 + w / M) * M : h} {
        EAGINE_ASSERT(_width > 0);
        EAGINE_ASSERT(_height > 0);
        _cells.resize(std_size(_width * _height));
    }

    auto width() const noexcept -> span_size_t {
        return _width;
    }

    auto height() const noexcept -> span_size_t {
        return _height;
    }

    auto get(span_size_t x, span_size_t y) const noexcept -> unsigned {
        EAGINE_ASSERT((x >= 0) && (x < width()));
        EAGINE_ASSERT((y >= 0) && (y < height()));
        return _cells[std_size(y * _width + x)];
    }

    friend auto
    operator<<(std::ostream& out, const basic_sudoku_tile_patch& that)
      -> std::ostream& {
        std::size_t k = 0;
        for(span_size_t y = 0; y < that._height; ++y) {
            for(span_size_t x = 0; x < that._width; ++x) {
                EAGINE_ASSERT(k < that._cells.size());
                out << std::setw(3) << unsigned(that._cells[k++]);
            }
            out << '\n';
        }
        return out;
    }

private:
    friend class basic_sudoku_tiling<S>;

    span_size_t _width{0};
    span_size_t _height{0};
    std::vector<std::uint8_t> _cells;
};
//------------------------------------------------------------------------------
template <unsigned S>
class basic_sudoku_tiling : basic_sudoku_solver<S> {
    static_assert(S > 2U);

public:
    using board_traits = basic_sudoku_board_traits<S>;
    using board_type = basic_sudoku_board<S>;

    basic_sudoku_tiling(const board_traits& traits)
      : _traits{traits} {
        _boards.emplace(
          std::make_tuple(0, 0), this->solve(traits.make_diagonal()));
    }

    basic_sudoku_tiling(const board_traits& traits, board_type board)
      : _traits{traits} {
        _boards.emplace(std::make_tuple(0, 0), this->solve(std::move(board)));
    }

    auto generate(int xmin, int ymin, int xmax, int ymax) -> auto& {
        for(int y = ymin; y <= ymax; ++y) {
            for(int x = xmin; x <= xmax; ++x) {
                _get(x, y);
            }
        }
        return *this;
    }

    auto fill(int xmin, int ymin, basic_sudoku_tile_patch<S>& patch)
      -> basic_sudoku_tile_patch<S>& {
        const auto ymax = ymin + patch.height() / (S * (S - 2));
        const auto xmax = xmin + patch.width() / (S * (S - 2));
        std::size_t k = 0;
        for(int y = ymax - 1; y >= ymin; --y) {
            for(unsigned by = 1; by < S - 1; ++by) {
                for(unsigned cy = 0; cy < S; ++cy) {
                    for(int x = xmin; x < xmax; ++x) {
                        auto& board = _get(x, y);
                        for(unsigned bx = 1; bx < S - 1; ++bx) {
                            for(unsigned cx = 0; cx < S; ++cx) {
                                EAGINE_ASSERT(k < patch._cells.size());
                                patch._cells[k++] =
                                  board.get({bx, by, cx, cy}).get_index();
                            }
                        }
                    }
                }
            }
        }
        return patch;
    }

    auto print(std::ostream& out, int xmin, int ymin, int xmax, int ymax)
      -> std::ostream& {
        for(int y = ymax; y >= ymin; --y) {
            for(unsigned by = 1; by < S - 1; ++by) {
                for(unsigned cy = 0; cy < S; ++cy) {
                    for(int x = xmin; x <= xmax; ++x) {
                        auto& board = _get(x, y);
                        for(unsigned bx = 1; bx < S - 1; ++bx) {
                            for(unsigned cx = 0; cx < S; ++cx) {
                                _traits.get().print(
                                  out, board.get({bx, by, cx, cy}));
                            }
                        }
                    }
                    out << '\n';
                }
            }
        }
        return out;
    }

private:
    auto _get(int x, int y) -> const board_type& {
        auto pos = _boards.find({x, y});
        if(pos == _boards.end()) {
            board_type added{_traits};
            if(y > 0) {
                if(x > 0) {
                    auto& left = _get(x - 1, y);
                    for(unsigned by = 0; by < S - 1; ++by) {
                        added.set_block(0U, by, left.get_block(S - 1, by));
                    }
                    auto& down = _get(x, y - 1);
                    for(unsigned bx = 1; bx < S; ++bx) {
                        added.set_block(bx, S - 1, down.get_block(bx, 0U));
                    }
                    pos = _emplace(x, y, added);
                } else if(x < 0) {
                    auto& right = _get(x + 1, y);
                    for(unsigned by = 0; by < S - 1; ++by) {
                        added.set_block(S - 1, by, right.get_block(0U, by));
                    }
                    auto& down = _get(x, y - 1);
                    for(unsigned bx = 0; bx < S - 1; ++bx) {
                        added.set_block(bx, S - 1, down.get_block(bx, 0U));
                    }
                    pos = _emplace(x, y, added);
                } else {
                    auto& down = _get(x, y - 1);
                    for(unsigned bx = 0; bx < S; ++bx) {
                        added.set_block(bx, S - 1, down.get_block(bx, 0U));
                    }
                    pos = _emplace(x, y, added);
                }
            } else if(y < 0) {
                if(x > 0) {
                    auto& left = _get(x - 1, y);
                    for(unsigned by = 1; by < S; ++by) {
                        added.set_block(0U, by, left.get_block(S - 1, by));
                    }
                    auto& up = _get(x, y + 1);
                    for(unsigned bx = 1; bx < S; ++bx) {
                        added.set_block(bx, 0U, up.get_block(bx, S - 1));
                    }
                    pos = _emplace(x, y, added);
                } else if(x < 0) {
                    auto& right = _get(x + 1, y);
                    for(unsigned by = 1; by < S; ++by) {
                        added.set_block(S - 1, by, right.get_block(0U, by));
                    }
                    auto& up = _get(x, y + 1);
                    for(unsigned bx = 0; bx < S - 1; ++bx) {
                        added.set_block(bx, 0U, up.get_block(bx, S - 1));
                    }
                    pos = _emplace(x, y, added);
                } else {
                    auto& up = _get(x, y + 1);
                    for(unsigned bx = 0; bx < S; ++bx) {
                        added.set_block(bx, 0U, up.get_block(bx, S - 1));
                    }
                    pos = _emplace(x, y, added);
                }
            } else {
                if(x > 0) {
                    auto& left = _get(x - 1, y);
                    for(unsigned by = 0; by < S; ++by) {
                        added.set_block(0U, by, left.get_block(S - 1, by));
                    }
                    pos = _emplace(x, y, added);
                } else if(x < 0) {
                    auto& right = _get(x + 1, y);
                    for(unsigned by = 0; by < S; ++by) {
                        added.set_block(S - 1, by, right.get_block(0U, by));
                    }
                    pos = _emplace(x, y, added);
                }
            }
        }
        return pos->second;
    }
    auto _emplace(int x, int y, board_type board) {
        return _boards
          .emplace(
            std::make_tuple(x, y), this->solve(board.calculate_alternatives()))
          .first;
    }

    std::reference_wrapper<const board_traits> _traits;
    flat_map<std::tuple<int, int>, basic_sudoku_board<S>> _boards;
};
//------------------------------------------------------------------------------
} // namespace eagine

#endif // EAGINE_SUDOKU_HPP