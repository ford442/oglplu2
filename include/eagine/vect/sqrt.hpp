/**
 *  @file eagine/vect/sqrt.hpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
#ifndef EAGINE_VECT_SQRT_HPP
#define EAGINE_VECT_SQRT_HPP

#include "data.hpp"
#include <cmath>

namespace eagine::vect {

template <typename T, int N, bool V>
struct sqrt {
    using _dT = data_t<T, N, V>;

    static _dT apply(_dT v) noexcept {
        for(int i = 0; i < N; ++i) {
            using std::sqrt;
            v[i] = T(sqrt(v[i]));
        }
        return v;
    }
};

} // namespace eagine::vect

#endif // EAGINE_VECT_SQRT_HPP
