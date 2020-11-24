#!/bin/bash
#  Copyright Matus Chochlik.
#  Distributed under the Boost Software License, Version 1.0.
#  See accompanying file LICENSE_1_0.txt or copy at
#   http://www.boost.org/LICENSE_1_0.txt
#
mkdir -p "${HOME}/.oglplus/" &&\
cp -v -r "$(dirname ${0})/.oglplus" "${HOME}/" &&\
cp -v -r "$(dirname ${0})/.config" "${HOME}/" &&\
echo "Success"
