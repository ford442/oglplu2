/**
 *  @file eagine/hexdump.inl
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#include <eagine/memory/address.hpp>
#include <cctype>
#include <iomanip>
#include <iostream>

namespace eagine {
//------------------------------------------------------------------------------
// hexdump::_to_hex_b
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
void hexdump::_to_hex_b(std::ostream& out, byte b) {
    static const char hd[16] = {'0',
                                '1',
                                '2',
                                '3',
                                '4',
                                '5',
                                '6',
                                '7',
                                '8',
                                '9',
                                'a',
                                'b',
                                'c',
                                'd',
                                'e',
                                'f'};
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    out << " " << hd[(b >> 4) & 0x0F] << hd[b & 0x0F];
}
//------------------------------------------------------------------------------
// ostream << hexdump
//------------------------------------------------------------------------------
EAGINE_LIB_FUNC
std::ostream& operator<<(std::ostream& out, const hexdump& hd) {
    out << std::endl;

    const byte* bgn = hd._mb.begin();
    const byte* end = hd._mb.end();
    const byte* row = memory::align_down(bgn, 16);

    while(row < end) {
        const auto adr = hd._offs ? memory::const_address(row)
                                  : memory::const_address(row - bgn);
        out << std::setw(20) << std::setfill('.');
        out << (static_cast<const void*>(adr.ptr()));
        out << "|";

        const byte* pos = row;
        for(unsigned b = 0; b < 16; ++b) {
            if(b == 8) {
                out << " ";
            }

            if(pos < bgn || pos >= end) {
                out << " ..";
            } else {
                hexdump::_to_hex_b(out, *pos);
            }
            ++pos;
        }

        out << " |";

        pos = row;
        for(unsigned b = 0; b < 16; ++b) {
            if(b == 8) {
                out << " ";
            }

            if(pos < bgn || pos >= end || !std::isprint(*pos)) {
                out << ".";
            } else {
                out << char(*pos);
            }
            ++pos;
        }

        row += 16;

        out << "|" << std::endl;
    }

    return out;
}
//------------------------------------------------------------------------------
} // namespace eagine
