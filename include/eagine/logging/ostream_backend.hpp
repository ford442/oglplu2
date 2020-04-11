/**
 *  @file eagine/logging/ostream_backend.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_LOGGING_OSTREAM_BACKEND_HPP
#define EAGINE_LOGGING_OSTREAM_BACKEND_HPP

#include "../hexdump.hpp"
#include "backend.hpp"
#include <mutex>
#include <ostream>

namespace eagine {
//------------------------------------------------------------------------------
template <typename Lockable = std::mutex>
class ostream_log_backend : public logger_backend {
private:
    Lockable _lockable{};
    std::ostream& _out;
    log_event_severity _min_severity;
    const std::chrono::steady_clock::time_point _start;

public:
    ostream_log_backend(
      std::ostream& out, log_event_severity min_severity) noexcept
      : _out{out}
      , _min_severity{min_severity}
      , _start{std::chrono::steady_clock::now()} {
        try {
            std::unique_lock lock{_lockable};
            _out << "<?xml version='1.0' encoding='UTF-8'?>\n";
            _out << "<log start='" << _start.time_since_epoch().count()
                 << "'>\n";
        } catch(...) {
        }
    }

    ostream_log_backend(ostream_log_backend&&) = delete;
    ostream_log_backend(const ostream_log_backend&) = delete;
    ostream_log_backend& operator=(ostream_log_backend&&) = delete;
    ostream_log_backend& operator=(const ostream_log_backend&) = delete;

    logger_backend* entry_backend(
      identifier, log_event_severity severity) noexcept final {
        if(severity >= _min_severity) {
            return this;
        }
        return nullptr;
    }

    void enter_scope(identifier scope) noexcept final {
        try {
            _lockable.lock();
            _out << "<s name='" << scope.name() << "'>\n";
        } catch(...) {
        }
    }

    void leave_scope(identifier) noexcept final {
        try {
            _lockable.lock();
            _out << "</s>\n";
        } catch(...) {
        }
    }

    bool begin_message(
      identifier source,
      log_event_severity severity,
      string_view format) noexcept final {
        try {
            _lockable.lock();
            const auto now = std::chrono::steady_clock::now();
            _out << "<m";
            _out << " lvl='" << enumerator_name(severity) << "'";
            _out << " src='" << source.name() << "'";
            _out << " ts='" << (now - _start).count() << "'";
            _out << ">";
            _out << "<f>" << format << "</f>";
        } catch(...) {
        }
        return true;
    }

    void add_identifier(
      identifier arg, identifier, identifier value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='id'>" << value.name()
                 << "</a>";
        } catch(...) {
        }
    }

    void add_bool(identifier arg, identifier, bool value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='bl'>"
                 << (value ? "true" : "false") << "</a>";
        } catch(...) {
        }
    }

    void add_integer(
      identifier arg, identifier, std::intmax_t value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='int'>" << value << "</a>";
        } catch(...) {
        }
    }

    void add_unsigned(
      identifier arg, identifier, std::uintmax_t value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='uint'>" << value << "</a>";
        } catch(...) {
        }
    }

    void add_float(identifier arg, identifier, float value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='real'>" << value << "</a>";
        } catch(...) {
        }
    }

    void add_duration(
      identifier arg,
      identifier,
      std::chrono::duration<float> value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='dur' u='s'>"
                 << value.count() << "</a>";
        } catch(...) {
        }
    }

    void add_blob(
      identifier arg, identifier, memory::const_block value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='blk'>" << hexdump(value)
                 << "</a>";
        } catch(...) {
        }
    }

    void add_string(
      identifier arg, identifier, string_view value) noexcept final {
        try {
            _out << "<a n='" << arg.name() << "' t='str'>" << value << "</a>";
        } catch(...) {
        }
    }

    void finish_message() noexcept final {
        try {
            _out << "</m>\n";
            _lockable.unlock();
        } catch(...) {
        }
    }

    ~ostream_log_backend() noexcept final {
        try {
            std::unique_lock lock{_lockable};
            _out << "</log>\n" << std::flush;
        } catch(...) {
        }
    }
};
//------------------------------------------------------------------------------
} // namespace eagine

#endif // EAGINE_LOGGING_OSTREAM_BACKEND_HPP
