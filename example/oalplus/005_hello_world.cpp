/**
 *  example oalplus/005_hello_world.cpp
 *
 *  Copyright Matus Chochlik.
 *  Distributed under the Boost Software License, Version 1.0.
 *  See accompanying file LICENSE_1_0.txt or copy at
 *   http://www.boost.org/LICENSE_1_0.txt
 */

#include <oalplus/al.hpp>
#include <oalplus/al_api.hpp>
#include <oalplus/alc_api.hpp>
#include <oalplus/alut_api.hpp>
#include <iostream>

int main(int argc, char** argv) {
    using namespace eagine;
    using namespace eagine::oalp;

    alc_api alc;

    if(auto open_dev_res = alc.open_device()) {
        auto& device = eagine::extract(open_dev_res);
        auto cleanup_dev = alc.close_device.raii(device);

        if(auto crt_ctx_res = alc.create_context(device)) {
            auto& context = extract(crt_ctx_res);
            auto cleanup_ctx = alc.destroy_context.raii(device, context);

            alc.MakeContextCurrent(context);

            al_api al;
            alut_api alut;

            if(alut.init(&argc, argv)) {
                auto do_exit = alut.exit.raii();
                auto opt_src = al.gen_sources();
                auto del_src = al.delete_sources.raii_opt(opt_src);
                auto opt_buf = alut.create_buffer_hello_world();
                auto del_buf = al.delete_buffers.raii_opt(opt_buf);

                if(opt_src && opt_buf) {
                    al.listener_i(al.position, 0, 0, 1);
                    // TODO: finish this
                    std::cerr << "this example is work in progress."
                              << std::endl;
                }
            }
        }
    }

    return 0;
}
