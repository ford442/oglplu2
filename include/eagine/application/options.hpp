/**
 *  @file eagine/application/options.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_APPLICATION_OPTIONS_HPP
#define EAGINE_APPLICATION_OPTIONS_HPP

#include "../main_ctx_object.hpp"
#include "../string_span.hpp"
#include "../valid_if/between.hpp"
#include "../valid_if/nonnegative.hpp"
#include "../valid_if/one_of.hpp"
#include "../valid_if/positive.hpp"
#include "types.hpp"

namespace eagine::application {

class launch_options : public main_ctx_object {
public:
    launch_options(main_ctx_parent parent) noexcept
      : main_ctx_object(EAGINE_ID(LaunchOpts), parent) {}

    auto application_title() const noexcept -> string_view {
        if(_app_title.empty()) {
            return main_context().app_name();
        }
        return {_app_title};
    }

    auto no_video() noexcept -> auto& {
        _requires_video = false;
        return *this;
    }

    auto require_video(
      video_context_kind kind = video_context_kind::opengl) noexcept -> auto& {
        _video_kind = kind;
        _requires_video = true;
        return *this;
    }

    auto required_video_kind() const noexcept
      -> optionally_valid<video_context_kind> {
        return {_video_kind, _requires_video};
    }

    auto no_audio() noexcept -> auto& {
        _requires_audio = false;
        return *this;
    }

    auto require_audio(
      audio_context_kind kind = audio_context_kind::openal) noexcept -> auto& {
        _audio_kind = kind;
        _requires_audio = true;
        return *this;
    }

    auto required_audio_kind() const noexcept
      -> optionally_valid<audio_context_kind> {
        return {_audio_kind, _requires_audio};
    }

    auto no_input() noexcept -> auto& {
        _requires_input = false;
        return *this;
    }

    auto require_input() noexcept -> auto& {
        _requires_input = true;
        return *this;
    }

    auto required_input() const noexcept {
        return _requires_input;
    }

    using valid_surface_size = valid_if_positive<int>;
    auto surface_size(
      const valid_surface_size& width,
      const valid_surface_size& height) noexcept -> auto& {
        _surface_width = extract(width);
        _surface_height = extract(height);
        return *this;
    }

    auto surface_width() const noexcept {
        return _surface_width;
    }

    auto surface_height() const noexcept {
        return _surface_height;
    }

    using valid_samples = valid_if_nonnegative<int>;
    auto samples(const valid_samples& value) noexcept -> auto& {
        _samples = extract(value);
        return *this;
    }

    auto samples_dont_care() noexcept -> auto& {
        _samples = 0;
        return *this;
    }

    auto samples() const noexcept -> valid_if_positive<int> {
        return _samples;
    }

    using valid_color_bits = valid_if_between<int, 0, 16>;
    auto color_bits(const valid_color_bits& value) noexcept -> auto& {
        _color_bits = extract(value);
        return *this;
    }

    auto color_bits() const noexcept -> int {
        return _color_bits;
    }

    using valid_alpha_bits = valid_if_between<int, 0, 16>;
    auto alpha_bits(const valid_alpha_bits& value) noexcept -> auto& {
        _alpha_bits = extract(value);
        return *this;
    }

    auto with_alpha() noexcept -> auto& {
        _alpha_bits = 8;
        return *this;
    }

    auto alpha_bits() const noexcept -> int {
        return _alpha_bits;
    }

    using valid_depth_bits = valid_if_between<int, 0, 32>;
    auto depth_bits(const valid_depth_bits& value) noexcept -> auto& {
        _depth_bits = extract(value);
        return *this;
    }

    auto with_depth() noexcept -> auto& {
        _depth_bits = 24;
        return *this;
    }

    auto depth_bits() const noexcept -> int {
        return _depth_bits;
    }

    using valid_stencil_bits = valid_if_one_of<int, 0, 8>;
    auto stencil_bits(const valid_stencil_bits& value) noexcept -> auto& {
        _stencil_bits = extract(value);
        return *this;
    }

    auto with_stencil() noexcept -> auto& {
        _stencil_bits = 8;
        return *this;
    }

    auto stencil_bits() const noexcept -> int {
        return _stencil_bits;
    }

private:
    std::string _app_title;

    video_context_kind _video_kind{video_context_kind::opengl};
    audio_context_kind _audio_kind{audio_context_kind::openal};

    int _surface_width{
      cfg_extr<valid_surface_size>("application.video.surface.width", 1280)};

    int _surface_height{
      cfg_extr<valid_surface_size>("application.video.surface.height", 800)};

    int _samples{cfg_extr<valid_samples>("application.video.samples", 0)};

    int _color_bits{
      cfg_extr<valid_color_bits>("application.video.color_bits", 8)};

    int _alpha_bits{
      cfg_extr<valid_alpha_bits>("application.video.alpha_bits", 0)};

    int _depth_bits{
      cfg_extr<valid_depth_bits>("application.video.depth_bits", 24)};

    int _stencil_bits{
      cfg_extr<valid_stencil_bits>("application.video.stencil_bits", 0)};

    bool _requires_input{cfg_init("application.input.required", false)};
    bool _requires_video{cfg_init("application.video.required", true)};
    bool _requires_audio{cfg_init("application.audio.required", false)};
    bool _gl_debug_context{cfg_init("application.video.opengl.debug", false)};
    bool _gl_compat_context{
      cfg_init("application.video.opengl.compatibility", false)};
};

} // namespace eagine::application

#endif