//  File implement/oalplus/enum/value_names_alut.inl
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the enumeration source files in 'source/enums/oalplus'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt
//

#include <eagine/maybe_unused.hpp>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4065)
#endif //_MSC_VER

namespace oalplus {

OALPLUS_LIB_FUNC
string_view get_enum_value_name( // NOLINT(hicpp-function-size)

  const any_enum_value_alut& aev) noexcept {
#ifdef ALUT_ERROR_ALC_ERROR_ON_ENTRY
    static const char* s_ERROR_ALC_ERROR_ON_ENTRY = "ERROR_ALC_ERROR_ON_ENTRY";
#endif
#ifdef ALUT_ERROR_AL_ERROR_ON_ENTRY
    static const char* s_ERROR_AL_ERROR_ON_ENTRY = "ERROR_AL_ERROR_ON_ENTRY";
#endif
#ifdef ALUT_ERROR_BUFFER_DATA
    static const char* s_ERROR_BUFFER_DATA = "ERROR_BUFFER_DATA";
#endif
#ifdef ALUT_ERROR_CLOSE_DEVICE
    static const char* s_ERROR_CLOSE_DEVICE = "ERROR_CLOSE_DEVICE";
#endif
#ifdef ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA
    static const char* s_ERROR_CORRUPT_OR_TRUNCATED_DATA =
      "ERROR_CORRUPT_OR_TRUNCATED_DATA";
#endif
#ifdef ALUT_ERROR_CREATE_CONTEXT
    static const char* s_ERROR_CREATE_CONTEXT = "ERROR_CREATE_CONTEXT";
#endif
#ifdef ALUT_ERROR_DESTROY_CONTEXT
    static const char* s_ERROR_DESTROY_CONTEXT = "ERROR_DESTROY_CONTEXT";
#endif
#ifdef ALUT_ERROR_GEN_BUFFERS
    static const char* s_ERROR_GEN_BUFFERS = "ERROR_GEN_BUFFERS";
#endif
#ifdef ALUT_ERROR_INVALID_ENUM
    static const char* s_ERROR_INVALID_ENUM = "ERROR_INVALID_ENUM";
#endif
#ifdef ALUT_ERROR_INVALID_OPERATION
    static const char* s_ERROR_INVALID_OPERATION = "ERROR_INVALID_OPERATION";
#endif
#ifdef ALUT_ERROR_INVALID_VALUE
    static const char* s_ERROR_INVALID_VALUE = "ERROR_INVALID_VALUE";
#endif
#ifdef ALUT_ERROR_IO_ERROR
    static const char* s_ERROR_IO_ERROR = "ERROR_IO_ERROR";
#endif
#ifdef ALUT_ERROR_MAKE_CONTEXT_CURRENT
    static const char* s_ERROR_MAKE_CONTEXT_CURRENT =
      "ERROR_MAKE_CONTEXT_CURRENT";
#endif
#ifdef ALUT_ERROR_NO_CURRENT_CONTEXT
    static const char* s_ERROR_NO_CURRENT_CONTEXT = "ERROR_NO_CURRENT_CONTEXT";
#endif
#ifdef ALUT_ERROR_NO_ERROR
    static const char* s_ERROR_NO_ERROR = "ERROR_NO_ERROR";
#endif
#ifdef ALUT_ERROR_OPEN_DEVICE
    static const char* s_ERROR_OPEN_DEVICE = "ERROR_OPEN_DEVICE";
#endif
#ifdef ALUT_ERROR_OUT_OF_MEMORY
    static const char* s_ERROR_OUT_OF_MEMORY = "ERROR_OUT_OF_MEMORY";
#endif
#ifdef ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE
    static const char* s_ERROR_UNSUPPORTED_FILE_SUBTYPE =
      "ERROR_UNSUPPORTED_FILE_SUBTYPE";
#endif
#ifdef ALUT_ERROR_UNSUPPORTED_FILE_TYPE
    static const char* s_ERROR_UNSUPPORTED_FILE_TYPE =
      "ERROR_UNSUPPORTED_FILE_TYPE";
#endif

    switch(aev._type_id) {
        case 0: /* alut_error_code */
            switch(ALenum(aev._value)) {
#ifdef ALUT_ERROR_ALC_ERROR_ON_ENTRY
                case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
                    return {s_ERROR_ALC_ERROR_ON_ENTRY, 24};
#endif
#ifdef ALUT_ERROR_AL_ERROR_ON_ENTRY
                case ALUT_ERROR_AL_ERROR_ON_ENTRY:
                    return {s_ERROR_AL_ERROR_ON_ENTRY, 23};
#endif
#ifdef ALUT_ERROR_BUFFER_DATA
                case ALUT_ERROR_BUFFER_DATA:
                    return {s_ERROR_BUFFER_DATA, 17};
#endif
#ifdef ALUT_ERROR_CLOSE_DEVICE
                case ALUT_ERROR_CLOSE_DEVICE:
                    return {s_ERROR_CLOSE_DEVICE, 18};
#endif
#ifdef ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA
                case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
                    return {s_ERROR_CORRUPT_OR_TRUNCATED_DATA, 31};
#endif
#ifdef ALUT_ERROR_CREATE_CONTEXT
                case ALUT_ERROR_CREATE_CONTEXT:
                    return {s_ERROR_CREATE_CONTEXT, 20};
#endif
#ifdef ALUT_ERROR_DESTROY_CONTEXT
                case ALUT_ERROR_DESTROY_CONTEXT:
                    return {s_ERROR_DESTROY_CONTEXT, 21};
#endif
#ifdef ALUT_ERROR_GEN_BUFFERS
                case ALUT_ERROR_GEN_BUFFERS:
                    return {s_ERROR_GEN_BUFFERS, 17};
#endif
#ifdef ALUT_ERROR_INVALID_ENUM
                case ALUT_ERROR_INVALID_ENUM:
                    return {s_ERROR_INVALID_ENUM, 18};
#endif
#ifdef ALUT_ERROR_INVALID_OPERATION
                case ALUT_ERROR_INVALID_OPERATION:
                    return {s_ERROR_INVALID_OPERATION, 23};
#endif
#ifdef ALUT_ERROR_INVALID_VALUE
                case ALUT_ERROR_INVALID_VALUE:
                    return {s_ERROR_INVALID_VALUE, 19};
#endif
#ifdef ALUT_ERROR_IO_ERROR
                case ALUT_ERROR_IO_ERROR:
                    return {s_ERROR_IO_ERROR, 14};
#endif
#ifdef ALUT_ERROR_MAKE_CONTEXT_CURRENT
                case ALUT_ERROR_MAKE_CONTEXT_CURRENT:
                    return {s_ERROR_MAKE_CONTEXT_CURRENT, 26};
#endif
#ifdef ALUT_ERROR_NO_CURRENT_CONTEXT
                case ALUT_ERROR_NO_CURRENT_CONTEXT:
                    return {s_ERROR_NO_CURRENT_CONTEXT, 24};
#endif
#ifdef ALUT_ERROR_NO_ERROR
                case ALUT_ERROR_NO_ERROR:
                    return {s_ERROR_NO_ERROR, 14};
#endif
#ifdef ALUT_ERROR_OPEN_DEVICE
                case ALUT_ERROR_OPEN_DEVICE:
                    return {s_ERROR_OPEN_DEVICE, 17};
#endif
#ifdef ALUT_ERROR_OUT_OF_MEMORY
                case ALUT_ERROR_OUT_OF_MEMORY:
                    return {s_ERROR_OUT_OF_MEMORY, 19};
#endif
#ifdef ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE
                case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
                    return {s_ERROR_UNSUPPORTED_FILE_SUBTYPE, 30};
#endif
#ifdef ALUT_ERROR_UNSUPPORTED_FILE_TYPE
                case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
                    return {s_ERROR_UNSUPPORTED_FILE_TYPE, 27};
#endif
                default:;
            }
            break;
        default:;
    }

    EAGINE_MAYBE_UNUSED(aev);
    return {};
}

} // namespace oalplus
#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER
