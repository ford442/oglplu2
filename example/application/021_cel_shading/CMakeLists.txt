# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
#
oglplus_add_glsl_program(cel_shading)
add_executable(application-021_cel_shading main.cpp resources.cpp)
eagine_embed_packed_target_resources(
	application-021_cel_shading
	cel_shading.oglpprog
)
eagine_add_exe_analysis(application-021_cel_shading)

add_dependencies(application-021_cel_shading application-examples)

target_link_libraries(
	application-021_cel_shading
	PUBLIC eagine eagine-application oglplus
)

set_target_properties(
	application-021_cel_shading
	PROPERTIES FOLDER "Example/Application"
)

install(
	TARGETS application-021_cel_shading
	DESTINATION share/oglplus/examples
)
