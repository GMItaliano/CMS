cmake_minimum_required(VERSION 3.0)
project(CMS_test)

find_package(OpenCV REQUIRED)

file(GLOB all_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp"   
)

add_executable(CMS_test main.cpp house_system.cpp livestream_ctrl.cpp ${all_SRCS})

target_link_libraries(test_opencv ${OpenCV_LIBS})