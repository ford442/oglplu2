/**
 *  .file oglplus/example/params.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_EXAMPLE_PARAMS_HPP
#define OGLPLUS_EXAMPLE_PARAMS_HPP

#include <eagine/assert.hpp>
#include <eagine/quantities.hpp>
#include <eagine/string_span.hpp>
#include <eagine/valid_if/not_empty.hpp>
#include <eagine/valid_if/positive.hpp>

namespace eagine {

enum class example_resource_type {
    texture_image,
    shader_source,
    program_source
};

class example_params {
private:
    unsigned _rand_seed{0};

    string_view _exec_cmd{};
    string_view _framedump_prefix{};
    string_view _screenshot_path{};
    float _screenshot_time{3.F};
    float _fixed_fps{30.F};

    int _x_pos{64};
    int _y_pos{64};

    int _x_tiles{1};
    int _y_tiles{1};

    int _samples{4};

    int _color_bits{8};
    int _alpha_bits{0};
    int _depth_bits{24};
    int _stencil_bits{0};

    bool _compat_ctxt{false};
    bool _debug_ctxt{true};
    bool _auto_tiles{true};
    bool _demo_mode{false};
    bool _high_quality{false};

public:
    example_params() noexcept = default;

    example_params& exec_command(valid_if_not_empty<string_view> cmd) {
        _exec_cmd = cmd.value();
        return *this;
    }

    string_view exec_command() const noexcept {
        return _exec_cmd;
    }

    bool is_readable_file(string_view path) const noexcept;

    valid_if_not_empty<std::string> find_resource_file_path(
      string_view res_group, string_view res_name) const;

    valid_if_not_empty<std::string> find_resource_file_path(
      string_view res_name) const {
        return find_resource_file_path(string_view(), res_name);
    }

    valid_if_not_empty<std::string> find_resource_file_path(
      example_resource_type, string_view res_name) const;

    std::string get_resource_file_path(
      example_resource_type, string_view res_name) const;

    example_params& screenshot_path(valid_if_not_empty<string_view> path) {
        _screenshot_path = path.value();
        return *this;
    }

    string_view screenshot_path() const noexcept {
        return _screenshot_path;
    }

    bool doing_screenshot() const noexcept {
        return !_screenshot_path.empty();
    }

    example_params& screenshot_time(seconds_t<float> ss_time) {
        _screenshot_time = value(ss_time);
        return *this;
    }

    seconds_t<float> screenshot_time() const noexcept {
        return seconds_(_screenshot_time);
    }

    example_params& framedump_prefix(valid_if_not_empty<string_view> prefix) {
        _framedump_prefix = prefix.value();
        return *this;
    }

    string_view framedump_prefix() const noexcept {
        return _framedump_prefix;
    }

    bool doing_framedump() const noexcept {
        return !_framedump_prefix.empty();
    }

    bool fixed_framerate() const noexcept {
        return doing_screenshot() || doing_framedump();
    }

    example_params& fixed_fps(valid_if_positive<float> fps) noexcept {
        _fixed_fps = fps.value();
        return *this;
    }

    float frame_time() const noexcept {
        return 1.0F / _fixed_fps;
    }

    example_params& window_x_pos(int pos) noexcept {
        _x_pos = pos;
        return *this;
    }

    int window_x_pos() const noexcept {
        return _x_pos;
    }

    example_params& window_y_pos(int pos) noexcept {
        _y_pos = pos;
        return *this;
    }

    int window_y_pos() const noexcept {
        return _y_pos;
    }

    example_params& rand_seed(unsigned seed) noexcept {
        _rand_seed = seed;
        return *this;
    }

    valid_if_positive<unsigned> rand_seed() const noexcept {
        return {_rand_seed};
    }

    example_params& compatibility_context(bool v) noexcept {
        _compat_ctxt = v;
        return *this;
    }

    bool compatibility_context() const noexcept {
        return _compat_ctxt;
    }

    example_params& debugging_context(bool v) noexcept {
        _debug_ctxt = v;
        return *this;
    }

    bool debugging_context() const noexcept {
        return _debug_ctxt;
    }

    example_params& auto_tiles(bool v) noexcept {
        _auto_tiles = v;
        return *this;
    }

    bool auto_tiles() const noexcept {
        return _auto_tiles;
    }

    example_params& x_tiles(valid_if_positive<int> n) noexcept {
        _x_tiles = n.value();
        return *this;
    }

    int x_tiles() const noexcept {
        return _x_tiles;
    }

    example_params& y_tiles(valid_if_positive<int> n) noexcept {
        _y_tiles = n.value();
        return *this;
    }

    int y_tiles() const noexcept {
        return _y_tiles;
    }

    example_params& demo_mode(bool v) noexcept {
        _demo_mode = v;
        return *this;
    }

    bool demo_mode() const noexcept {
        return _demo_mode;
    }

    example_params& high_quality(bool v) noexcept {
        _high_quality = v;
        return *this;
    }

    bool high_quality() const noexcept {
        return _high_quality;
    }

    example_params& samples(valid_if_positive<int> n) noexcept {
        _samples = n.value();
        return *this;
    }

    example_params& samples_dont_care() noexcept {
        _samples = 0;
        return *this;
    }

    valid_if_positive<int> samples() const noexcept {
        return _samples;
    }

    example_params& color_bits(int n) noexcept {
        EAGINE_ASSERT(n >= 0);
        _color_bits = n;
        return *this;
    }

    int color_bits() const noexcept {
        return _color_bits;
    }

    example_params& with_alpha(bool v) noexcept {
        _alpha_bits = v ? 8 : 0;
        return *this;
    }

    int alpha_bits() const noexcept {
        return _alpha_bits;
    }

    example_params& depth_buffer(bool v) noexcept {
        _depth_bits = v ? 24 : 0;
        return *this;
    }

    int depth_bits() const noexcept {
        return _depth_bits;
    }

    bool depth_buffer() const noexcept {
        return _depth_bits > 0;
    }

    example_params& stencil_buffer(bool v) noexcept {
        _stencil_bits = v ? 8 : 0;
        return *this;
    }

    int stencil_bits() const noexcept {
        return _stencil_bits;
    }

    bool stencil_buffer() const noexcept {
        return _stencil_bits > 0;
    }
};

extern void adjust_params(example_params&);

} // namespace eagine

#endif // OGLPLUS_EXAMPLE_PARAMS_HPP
