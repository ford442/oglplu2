/**
 *  @file eagine/timeout.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_TIMEOUT_HPP
#define EAGINE_TIMEOUT_HPP

#include <chrono>

namespace eagine {
//------------------------------------------------------------------------------
class time_measure {
    using _clock = std::chrono::steady_clock;

public:
    time_measure() noexcept
      : _start{_clock::now()} {}

    auto elapsed_time() const noexcept {
        return _clock::now() - _start;
    }

    auto seconds() const noexcept {
        return std::chrono::duration_cast<std::chrono::duration<float>>(
          elapsed_time());
    }

private:
    _clock::time_point _start{};
};
//------------------------------------------------------------------------------
class timeout {
    using _clock = std::chrono::steady_clock;

public:
    timeout() noexcept
      : _timeout{_clock::now()} {}

    timeout(_clock::duration duration, _clock::duration initial) noexcept
      : _duration{duration}
      , _timeout{_clock::now() + initial} {}

    timeout(_clock::duration duration, nothing_t) noexcept
      : timeout{duration, _clock::duration::zero()} {}

    timeout(_clock::duration duration) noexcept
      : timeout{duration, duration} {}

    template <typename R, typename P>
    timeout(std::chrono::duration<R, P> duration) noexcept
      : timeout{std::chrono::duration_cast<_clock::duration>(duration)} {}

    auto reset() noexcept -> auto& {
        _timeout = std::chrono::steady_clock::now() + _duration;
        return *this;
    }

    auto reset(_clock::duration duration) noexcept -> auto& {
        _duration = duration;
        _timeout = std::chrono::steady_clock::now() + _duration;
        return *this;
    }

    auto elapsed_time() const noexcept {
        return _clock::now() - _timeout + _duration;
    }

    auto is_expired() const noexcept -> bool {
        return _clock::now() >= _timeout;
    }

    explicit operator bool() const noexcept {
        return is_expired();
    }

private:
    _clock::duration _duration{};
    _clock::time_point _timeout{};
};
//------------------------------------------------------------------------------
class resetting_timeout : public timeout {
public:
    using timeout::timeout;

    explicit operator bool() noexcept {
        const auto result = is_expired();
        reset();
        return result;
    }
};

} // namespace eagine

#endif // EAGINE_TIMEOUT_HPP
