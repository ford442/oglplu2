/**
 *  @file eagine/shapes/centered.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef EAGINE_SHAPES_CENTERED_HPP
#define EAGINE_SHAPES_CENTERED_HPP

#include "delegated.hpp"
#include <eagine/config/basic.hpp>
#include <utility>

namespace eagine {
namespace shapes {
//------------------------------------------------------------------------------
class centered_gen : public delegated_gen {

public:
    centered_gen(std::unique_ptr<generator_intf>&& gen) noexcept
      : delegated_gen(std::move(gen)) {
    }

    void attrib_values(vertex_attrib_kind attr, span<float> dest) override;

    math::sphere<float, true> bounding_sphere() override;
};
//------------------------------------------------------------------------------
static inline auto center(std::unique_ptr<generator_intf>&& gen) noexcept {
    return std::unique_ptr<generator_intf>(new centered_gen(std::move(gen)));
}
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace eagine

#if !EAGINE_LINK_LIBRARY || defined(EAGINE_IMPLEMENTING_LIBRARY)
#include <eagine/shapes/centered.inl>
#endif

#endif // EAGINE_SHAPES_CENTERED_HPP
