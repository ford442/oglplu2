/**
 *  @file oglplus/ctxt/shader.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef OGLPLUS_CTXT_SHADER_1509260923_HPP
#define OGLPLUS_CTXT_SHADER_1509260923_HPP

#include "../shader.hpp"
#include "../glsl/source_ref.hpp"

namespace oglplus {
namespace ctxt {

struct shader_ops
{
	static
	outcome<void>
	shader_source(shader_name shader, const glsl_source_ref& source)
	noexcept
	{
		OGLPLUS_GLFUNC(ShaderSource)(
			get_raw_name(shader),
			source.count(),
			source.parts(),
			source.lengths()
		);
		OGLPLUS_VERIFY(ShaderSource, gl_object(shader), warning);
		return {};
	}

	static
	outcome<void>
	compile(shader_name shader)
	noexcept
	{
		OGLPLUS_GLFUNC(CompileShader)(get_raw_name(shader));
		OGLPLUS_VERIFY(CompileShader, gl_object(shader), warning);
		return {};
	}
};

} // namespace ctxt
} // namespace oglplus

#endif // include guard
