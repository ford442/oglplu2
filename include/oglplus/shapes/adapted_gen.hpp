/**
 *  @file oglplus/shapes/adapted_gen.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef OGLPLUS_SHAPES_ADAPTED_GEN_HPP
#define OGLPLUS_SHAPES_ADAPTED_GEN_HPP

#include "../gl_api/api.hpp"
#include "../math/primitives.hpp"
#include "drawing.hpp"
#include <eagine/memory/block.hpp>
#include <eagine/shapes/gen_base.hpp>
#include <eagine/shapes/vertex_attrib.hpp>
#include <eagine/span.hpp>
#include <memory>

namespace eagine {
namespace oglp {
namespace shapes {
//------------------------------------------------------------------------------
class adapted_generator {
private:
    using generator_intf = eagine::shapes::generator_intf;
    std::unique_ptr<generator_intf> _gen{};

public:
    template <typename A>
    adapted_generator(
      const basic_gl_api<A>&, std::unique_ptr<generator_intf>&&);

    template <
      typename Gen,
      typename = std::enable_if_t<std::is_base_of_v<generator_intf, Gen>>>
    adapted_generator(Gen gen)
      : adapted_generator(std::make_unique<Gen>(std::move(gen))) {
    }

    span_size_t vertex_count() const {
        return _gen->vertex_count();
    }

    span_size_t values_per_vertex(
      eagine::shapes::vertex_attrib_kind attrib,
      span_size_t variant_index) const {
        return _gen->values_per_vertex(attrib, variant_index);
    }

    span_size_t value_count(
      eagine::shapes::vertex_attrib_kind attrib,
      span_size_t variant_index) const {
        return vertex_count() * values_per_vertex(attrib, variant_index);
    }

    span_size_t attrib_type_size(
      eagine::shapes::vertex_attrib_kind attrib,
      span_size_t variant_index) const {
        return type_size(_gen->attrib_type(attrib, variant_index));
    }

    span_size_t attrib_data_block_size(
      eagine::shapes::vertex_attrib_kind attrib,
      span_size_t variant_index) const {
        return value_count(attrib, variant_index) *
               attrib_type_size(attrib, variant_index);
    }

    void attrib_data(
      eagine::shapes::vertex_attrib_kind attrib,
      span_size_t variant_index,
      memory::block data) const;

    bool is_indexed() const {
        return _gen->index_count() > 0;
    }

    span_size_t index_count() const {
        return _gen->index_count();
    }

    span_size_t index_type_size() const {
        return type_size(_gen->index_type());
    }

    span_size_t index_data_block_size() const {
        return index_count() * index_type_size();
    }

    void index_data(memory::block data) const;

    span_size_t operation_count() const {
        return _gen->operation_count();
    }

    template <typename A>
    void instructions(const basic_gl_api<A>&, span<draw_operation>) const;

    sphere bounding_sphere() const {
        return _gen->bounding_sphere();
    }

    optionally_valid<float> ray_intersection(const line& ray) const {
        return _gen->ray_intersection(ray);
    }

    optionally_valid<float> ray_intersection(
      const optionally_valid<line>& opt_ray) const {
        if(opt_ray) {
            return ray_intersection(extract(opt_ray));
        }
        return {};
    }
};
//------------------------------------------------------------------------------
template <typename Generator>
class concrete_adapted_generator : public adapted_generator {
public:
    template <typename... P>
    concrete_adapted_generator(
      eagine::shapes::vertex_attrib_bits bits, P&&... p)
      : adapted_generator(Generator(bits, std::forward<P>(p)...)) {
    }
};
//------------------------------------------------------------------------------
} // namespace shapes
} // namespace oglp
} // namespace eagine

#include <oglplus/shapes/adapted_gen.inl>

#endif // OGLPLUS_SHAPES_ADAPTED_GEN_HPP
