# Copyright Matus Chochlik.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt

# router
add_executable(eagine-message_bus-router router.cpp)
eagine_add_exe_analysis(eagine-message_bus-router)
target_link_libraries(
	eagine-message_bus-router
	PUBLIC eagine
)

# bridge
add_executable(eagine-message_bus-bridge bridge.cpp)
eagine_add_exe_analysis(eagine-message_bus-bridge)
target_link_libraries(
	eagine-message_bus-bridge
	PUBLIC eagine
)

# pingable
add_executable(eagine-message_bus-pingable pingable.cpp)
eagine_add_exe_analysis(eagine-message_bus-pingable)
target_link_libraries(
	eagine-message_bus-pingable
	PUBLIC eagine
)

# pinger
add_executable(eagine-message_bus-pinger pinger.cpp)
eagine_add_exe_analysis(eagine-message_bus-pinger)
target_link_libraries(
	eagine-message_bus-pinger
	PUBLIC eagine
)

# file server
add_executable(eagine-message_bus-file_server file_server.cpp)
eagine_add_exe_analysis(eagine-message_bus-file_server)
target_link_libraries(
	eagine-message_bus-file_server
	PUBLIC eagine
)

# sudoku solver helper
add_executable(eagine-message_bus-sudoku_helper sudoku_helper.cpp)
eagine_add_exe_analysis(eagine-message_bus-sudoku_helper)
target_link_libraries(
	eagine-message_bus-sudoku_helper
	PUBLIC eagine
)

# sudoku tiling generator
add_executable(eagine-message_bus-sudoku_tiling sudoku_tiling.cpp)
eagine_add_exe_analysis(eagine-message_bus-sudoku_tiling)
target_link_libraries(
	eagine-message_bus-sudoku_tiling
	PUBLIC eagine
)

# scripts
install(
	PROGRAMS eagine-local-bridge.sh
	RENAME eagine-local-bridge
	DESTINATION bin
	COMPONENT tools
)
install(
	PROGRAMS eagine-sudoku-tiling.sh
	RENAME eagine-sudoku-tiling
	DESTINATION bin
	COMPONENT tools
)

find_package(Qt5 COMPONENTS Core Widgets Qml Quick)
if(Qt5_FOUND)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)
 	add_subdirectory(monitor)
 	add_subdirectory(tiling)
endif()
