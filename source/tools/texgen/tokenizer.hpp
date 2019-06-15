/**
 *  @file texgen/tokenizer.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef TEXGEN_TOKENIZER_HPP
#define TEXGEN_TOKENIZER_HPP

#include "input_stream.hpp"
#include "token_info.hpp"

namespace oglplus {
namespace texgen {
//------------------------------------------------------------------------------
class tokenizer {
public:
    tokenizer(input_stream input)
      : _input(std::move(input)) {
    }

    bool get_next(token_info&);

private:
    input_stream _input;
};
//------------------------------------------------------------------------------
} // namespace texgen
} // namespace oglplus

#endif // TEXGEN_TOKENIZER_HPP
