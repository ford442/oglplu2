/**
 *  @file eagine/application/state.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_APPLICATION_STATE_HPP
#define EAGINE_APPLICATION_STATE_HPP

#include "../main_ctx_object.hpp"
#include "../valid_if/positive.hpp"
#include "state_view.hpp"

namespace eagine::application {
//------------------------------------------------------------------------------
class context_state
  : public main_ctx_object
  , public context_state_view {

public:
    context_state(main_ctx_parent parent)
      : main_ctx_object(EAGINE_ID(AppliState), parent) {}

    auto advance_time() noexcept -> auto& {
        if(_fixed_fps) {
            _frame_time.advance(extract(_fixed_fps));
        } else {
            _frame_time.assign(
              std::chrono::duration<float>(run_time()).count());
        }
        _old_user_idle = _new_user_idle;
        _new_user_idle = true;
        return *this;
    }

private:
    valid_if_positive<float> _fixed_fps{
      cfg_extr<valid_if_positive<float>>("application.video.fixed_fps", 0.F)};
};
//------------------------------------------------------------------------------
} // namespace eagine::application

#endif
