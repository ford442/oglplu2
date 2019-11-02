#  Copyright Matus Chochlik.
#  Distributed under the Boost Software License, Version 1.0.
#  See accompanying file LICENSE_1_0.txt or copy at
#   http://www.boost.org/LICENSE_1_0.txt
#

# context init libs
set(OGLPLUS_USE_GLX 0)
set(OGLPLUS_USE_GLFW3 0)
set(OGLPLUS_USE_GLFW 0)
set(OGLPLUS_USE_SDL 0)
set(OGLPLUS_USE_GLUT 0)
#
#set(ALL_GL_INIT_LIBS GLX GLUT GLFW3 GLFW SDL WXGL EGL) TODO
set(ALL_GL_INIT_LIBS GLX GLFW3 GLFW SDL GLUT)

# API libs
set(OGLPLUS_USE_GLCOREARB_H 0)
set(OGLPLUS_USE_GL3_GL3_H 0)
set(OGLPLUS_USE_GLES3_GL3_H 0)
set(OGLPLUS_USE_GLES3_GL31_H 0)
set(OGLPLUS_USE_GLES3_GL32_H 0)
set(OGLPLUS_USE_GLEW 0)
set(OGLPLUS_USE_GL3W 0)
#
if(${WIN32})
	set(ALL_GL_API_LIBS
		GLEW GL3W GLCOREARB_H GL3_GL3_H
		GLES3_GL32_H GLES3_GL31_H GLES3_GL3_H
	)
else()
	set(ALL_GL_API_LIBS
		GLCOREARB_H GL3_GL3_H GLEW GL3W
		GLES3_GL32_H GLES3_GL31_H GLES3_GL3_H
	)
endif()
#
# The GL context initialization library
set(FORCE_GL_INIT_LIB "NONE" CACHE STRING "The GL context initialization library to be used.")
set_property(CACHE FORCE_GL_INIT_LIB PROPERTY STRINGS ${ALL_GL_INIT_LIBS})

# The GL API declaration library
set(FORCE_GL_API_LIB "NONE" CACHE STRING "The GL API definition library to be used.")
set_property(CACHE FORCE_GL_API_LIB PROPERTY STRINGS ${ALL_GL_API_LIBS})

# We'll need OpenGL API and context initialization/window-system library
foreach(KIND API INIT)
	if((NOT FORCE_GL_${KIND}_LIB) OR (${FORCE_GL_${KIND}_LIB} STREQUAL "NONE"))
		foreach(GL_${KIND}_LIB ${ALL_GL_${KIND}_LIBS})
			if(${GL_${KIND}_LIB}_FOUND)
				set(GL_LIB_HAS_ALL_DEPS true)
				string(TOLOWER ${GL_${KIND}_LIB} LIB_LC)
				# TODO
				#do_require_all_dependencies(
				#	"${CMAKE_CURRENT_SOURCE_DIR}/example/oglplus"
				#	"${LIB_LC}_main"
				#	GL_LIB_HAS_ALL_DEPS
				#)
				unset(LIB_LC)
				if(${GL_LIB_HAS_ALL_DEPS})
					set(OGLPLUS_GL_${KIND}_LIB ${GL_${KIND}_LIB})
					set(OGLPLUS_USE_${GL_${KIND}_LIB} 1)
					break()
				endif()
			endif()
		endforeach()
	else()
		foreach(GL_${KIND}_LIB ${ALL_GL_${KIND}_LIBS})
			if(${FORCE_GL_${KIND}_LIB} STREQUAL ${GL_${KIND}_LIB})
				if(${GL_${KIND}_LIB}_FOUND)
					set(OGLPLUS_GL_${KIND}_LIB ${GL_${KIND}_LIB})
					set(OGLPLUS_USE_${GL_${KIND}_LIB} 1)
					break()
				else()
					message(
						FATAL_ERROR
						"The '${GL_${KIND}_LIB}' library requested but not found"
					)
					return()
				endif()
			endif()
		endforeach()
	endif()
	file(
		WRITE "${PROJECT_BINARY_DIR}/cfg-GL_${KIND}_LIB"
		"${OGLPLUS_GL_${KIND}_LIB}"
	)
endforeach()


if(NOT OGLPLUS_GL_INIT_LIB)
	if(NOT NO_EXAMPLES)
		message(FATAL_ERROR "No OpenGL context initialization library found!")
	endif()
else()
	message(STATUS "Using ${OGLPLUS_GL_INIT_LIB} for GL context initialization.")
endif()

unset(OGLPLUS_GL_INCLUDE_DIRS)

if((${OGLPLUS_USE_GLCOREARB_H}) OR (${OGLPLUS_USE_GL3_GL3_H}))

	set(OGLPLUS_GL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${OPENGL_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using native OpenGL API library")
elseif((${OGLPLUS_USE_GLES3_GL32_H}))

	set(OGLPLUS_GL_INCLUDE_DIRS ${OPENGLES32_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${OPENGLES32_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${OPENGLES32_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using OpenGL|ES 3.2 API library")
elseif((${OGLPLUS_USE_GLES3_GL31_H}))

	set(OGLPLUS_GL_INCLUDE_DIRS ${OPENGLES31_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${OPENGLES31_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${OPENGLES31_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using OpenGL|ES 3.1 API library")
elseif((${OGLPLUS_USE_GLES3_GL3_H}))

	set(OGLPLUS_GL_INCLUDE_DIRS ${OPENGLES3_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${OPENGLES3_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${OPENGLES3_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using OpenGL|ES 3 API library")
elseif(${OGLPLUS_USE_GLEW})

	set(OGLPLUS_GL_INCLUDE_DIRS ${GLEW_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${GLEW_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using GLEW as GL API library")
elseif(${OGLPLUS_USE_GL3W})

  set(OGLPLUS_GL_INCLUDE_DIRS ${GL3W_INCLUDE_DIRS})
	include_directories(BEFORE SYSTEM ${GL3W_INCLUDE_DIRS})
	set(OGLPLUS_GL_LIBRARIES ${GL3W_LIBRARIES} ${OPENGL_LIBRARIES})
	set(OGLPLUS_GL_FOUND 1)

	message(STATUS "Using GL3W as GL API library")
else()
	message(FATAL_ERROR "A library including OpenGL symbols is required for OGLplus")
	return()
endif()

