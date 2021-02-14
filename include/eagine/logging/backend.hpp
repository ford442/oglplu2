/// @file
///
/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///

#ifndef EAGINE_LOGGING_BACKEND_HPP
#define EAGINE_LOGGING_BACKEND_HPP

#include "../memory/block.hpp"
#include "../memory/shared_alloc.hpp"
#include "../message_id.hpp"
#include "../string_span.hpp"
#include "severity.hpp"
#include <chrono>
#include <cstdint>

namespace eagine {
//------------------------------------------------------------------------------
template <typename T>
struct does_have_log_entry_adapter {
private:
    template <
      typename X,
      typename = decltype(
        adapt_log_entry_arg(std::declval<identifier>(), std::declval<X>()))>
    static auto _test(X*) -> std::true_type;
    static auto _test(...) -> std::false_type;

public:
    // NOLINTNEXTLINE(hicpp-vararg)
    using type = decltype(_test(static_cast<T*>(nullptr)));
};

template <typename T>
using has_log_entry_adapter_t = typename does_have_log_entry_adapter<T>::type;

template <typename T>
constexpr const bool has_log_entry_adapter_v =
  has_log_entry_adapter_t<T>::value;
//------------------------------------------------------------------------------
using logger_instance_id = std::uintptr_t;

struct logger_backend {

    logger_backend() noexcept = default;
    logger_backend(logger_backend&&) noexcept = default;
    logger_backend(const logger_backend&) noexcept = default;
    auto operator=(logger_backend&&) noexcept -> logger_backend& = default;
    auto operator=(const logger_backend&) noexcept -> logger_backend& = default;
    virtual ~logger_backend() noexcept = default;

    virtual auto allocator() noexcept -> memory::shared_byte_allocator = 0;

    virtual auto type_id() noexcept -> identifier = 0;

    virtual auto
    entry_backend(identifier source, log_event_severity severity) noexcept
      -> logger_backend* = 0;

    virtual void enter_scope(identifier scope) noexcept = 0;

    virtual void leave_scope(identifier scope) noexcept = 0;

    virtual void set_description(
      identifier source,
      logger_instance_id instance,
      string_view display_name,
      string_view description) noexcept = 0;

    virtual auto begin_message(
      identifier source,
      logger_instance_id instance,
      log_event_severity severity,
      string_view format) noexcept -> bool = 0;

    virtual void add_nothing(identifier arg, identifier tag) noexcept = 0;

    virtual void add_identifier(
      identifier arg,
      identifier tag,
      identifier value) noexcept = 0;

    virtual void
    add_message_id(identifier arg, identifier tag, message_id) noexcept = 0;

    virtual void
    add_bool(identifier arg, identifier tag, bool value) noexcept = 0;

    virtual void add_integer(
      identifier arg,
      identifier tag,
      std::intmax_t value) noexcept = 0;

    virtual void add_unsigned(
      identifier arg,
      identifier tag,
      std::uintmax_t value) noexcept = 0;

    virtual void
    add_float(identifier arg, identifier tag, float value) noexcept = 0;

    virtual void add_float(
      identifier arg,
      identifier tag,
      float min,
      float value,
      float max) noexcept = 0;

    virtual void add_duration(
      identifier arg,
      identifier tag,
      std::chrono::duration<float> value) noexcept = 0;

    virtual void
    add_string(identifier arg, identifier tag, string_view value) noexcept = 0;

    virtual void add_blob(
      identifier arg,
      identifier tag,
      memory::const_block value) noexcept = 0;

    template <typename T>
    auto add_adapted(identifier arg, const T& value)
      -> std::enable_if_t<has_log_entry_adapter_v<T>> {
        adapt_log_entry_arg(arg, value)(*this);
    }

    virtual void finish_message() noexcept = 0;

    virtual void finish_log() noexcept = 0;

    virtual void log_chart_sample(
      identifier source,
      logger_instance_id instance,
      identifier series,
      float value) noexcept = 0;
};
//------------------------------------------------------------------------------
} // namespace eagine

#endif // EAGINE_LOGGING_BACKEND_HPP
