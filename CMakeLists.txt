cmake_minimum_required(VERSION 3.22.1)
project(CMS_test)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Including all include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/main/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
    ${CMAKE_CURRENT_SOURCE_DIR}/daemon/
)

# All source files
# file(GLOB main_SRCS
#     "${CMAKE_CURRENT_SOURCE_DIR}/main/src/*.cpp"
# )

# Main process
add_executable(main.o main/main.cpp main/src/house_system.cpp) #${main_SRCS}

# All source files for daemon
# file(GLOB daemon_SRCS
#     "${CMAKE_CURRENT_SOURCE_DIR}/daemon/daemon.cpp"
# )

# Daemon process
add_executable(daemon.o daemon/maind.cpp daemon/Cdaemon.cpp) #${daemon_SRCS}
