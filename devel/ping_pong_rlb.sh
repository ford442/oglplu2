#!/bin/bash
# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
#
install_prefix="$(<$(dirname ${0})/../INSTALL_PREFIX)"
log_args=("--use-asio-log" "--min-log-severity" "info")
conn_type="--msg-bus-asio-local-stream"
ping_addr="/tmp/ping"
pong_addr="/tmp/pong"
#
pids=()
#
${install_prefix}/bin/eagine-message_bus-router \
	"${log_args[@]}" \
	${conn_type} \
	--msg-bus-router-address ${ping_addr} \
	--msg-bus-router-id-base 1000 \
	& pids+=($!)
sleep 1
${install_prefix}/bin/eagine-message_bus-router \
	"${log_args[@]}" \
	${conn_type} \
	--msg-bus-router-address ${pong_addr} \
	--msg-bus-router-id-base 2000 \
	& pids+=($!)
sleep 1
${install_prefix}/bin/eagine-local-bridge \
	"${log_args[@]}" \
	${conn_type} \
	-l --msg-bus-router-address ${ping_addr} \
	-r --msg-bus-router-address ${pong_addr} \
	& pids+=($!)
sleep 1
${install_prefix}/share/oglplus/examples/eagine-006_pong \
	"${log_args[@]}" \
	${conn_type} \
	--msg-bus-router-address ${pong_addr} \
	& pids+=($!)
sleep 1
${install_prefix}/share/oglplus/examples/eagine-006_ping \
	"${log_args[@]}" \
	--ping-count 1M \
	${conn_type} \
	--msg-bus-router-address ${ping_addr} \
	& pids+=($!)

for pid in ${pids[@]}
do wait ${pid}
done

