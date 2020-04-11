/**
 *  @file eagine/logging/backend.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_LOGGING_BACKEND_HPP
#define EAGINE_LOGGING_BACKEND_HPP

#include "../identifier.hpp"
#include "../memory/block.hpp"
#include "../string_span.hpp"
#include "severity.hpp"
#include <chrono>
#include <cstdint>

namespace eagine {
//------------------------------------------------------------------------------
struct logger_backend {
    logger_backend() noexcept = default;
    logger_backend(logger_backend&&) noexcept = default;
    logger_backend(const logger_backend&) noexcept = default;
    logger_backend& operator=(logger_backend&&) noexcept = default;
    logger_backend& operator=(const logger_backend&) noexcept = default;
    virtual ~logger_backend() noexcept = default;

    virtual logger_backend* entry_backend(
      identifier source, log_event_severity severity) noexcept = 0;

    virtual void enter_scope(identifier scope) noexcept = 0;

    virtual void leave_scope(identifier scope) noexcept = 0;

    virtual bool begin_message(
      identifier source,
      log_event_severity severity,
      string_view format) noexcept = 0;

    virtual void add_identifier(identifier arg, identifier value) noexcept = 0;

    virtual void add_integer(identifier arg, std::intmax_t value) noexcept = 0;

    virtual void add_unsigned(
      identifier arg, std::uintmax_t value) noexcept = 0;

    virtual void add_float(identifier arg, float value) noexcept = 0;

    virtual void add_string(identifier arg, string_view value) noexcept = 0;

    virtual void add_duration(
      identifier arg, std::chrono::duration<float> value) noexcept = 0;

    virtual void add_blob(
      identifier arg, memory::const_block value) noexcept = 0;

    virtual void finish_message() noexcept = 0;
};
//------------------------------------------------------------------------------
struct null_log_backend : logger_backend {
    logger_backend* entry_backend(
      identifier, log_event_severity) noexcept final {
        return nullptr;
    }

    void enter_scope(identifier) noexcept final {
    }

    void leave_scope(identifier) noexcept final {
    }

    bool begin_message(
      identifier, log_event_severity, string_view) noexcept final {
        return false;
    }

    void add_identifier(identifier, identifier) noexcept final {
    }

    void add_integer(identifier, std::intmax_t) noexcept final {
    }

    void add_unsigned(identifier, std::uintmax_t) noexcept final {
    }

    void add_float(identifier, float) noexcept final {
    }

    void add_string(identifier, string_view) noexcept final {
    }

    void add_duration(identifier, std::chrono::duration<float>) noexcept final {
    }

    void add_blob(identifier, memory::const_block) noexcept final {
    }

    void finish_message() noexcept final {
    }
};
//------------------------------------------------------------------------------
} // namespace eagine

#endif // EAGINE_LOGGING_BACKEND_HPP