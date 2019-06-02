/*
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */
// clang-format off
#include <oalplus/al.hpp>
#include <oalplus/buffer.hpp>
#include "helper/object_c.hpp"
// clang-format on

using namespace oalplus;

void oalplus_buffer_test_1() {
    oalplus_object_name_test<buffer_name>();
    oalplus_object_test<buffer>();
}

void oalplus_buffer_test_ops_1() {
    using namespace oalplus;

    oper::buffer_ops al;
    buffer buf;
    buffer_data_spec data;

    al.buffer_data(buf, data_format(), data, 12345);
    al.buffer_size(buf);
    al.buffer_bits(buf);
    al.buffer_channels(buf);
    al.buffer_frequency(buf);

    buf.data(data_format(), data, 12345);
    buf.size();
    buf.bits();
    buf.channels();
    buf.frequency();
}

// TODO
