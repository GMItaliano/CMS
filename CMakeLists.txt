cmake_minimum_required(VERSION 3.0)
project(CMS)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Find OpenAL package
find_package(OpenAL REQUIRED)

# Add your source files here
set(SOURCES
    house_system.cpp camera_sys.cpp microphone_sys.cpp livestream.cpp
)

# Create the executable
add_executable(CMS_test ${SOURCES})

# Link OpenCV and OpenAL libraries
target_link_libraries(CMS_test ${OpenCV_LIBS} ${OPENAL_LIBRARY})


