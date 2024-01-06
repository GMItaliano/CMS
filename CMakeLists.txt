        #if(FALSE)
        # CROSS COMPILE VERSION
        cmake_minimum_required(VERSION 3.22.1)
        project(CMS_test LANGUAGES CXX)

        # Compilers
        set(CMAKE_CXX_COMPILER "/home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++")
        set(CMAKE_CC_COMPILER "/home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-gcc")

        # Bin files to bin dir
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

        include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/main/inc
        /home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include/python3.9
        )

        set(SOURCES
        main/main.cpp
        main/src/house_system.cpp
        main/src/database_sys.cpp
        main/src/relay_sys.cpp
        main/src/livestream_ctrl.cpp
        )

        # Main
        add_executable(mainlocal.elf ${SOURCES})
        target_compile_features(mainlocal.elf PRIVATE cxx_std_11)

        # Linking Libraries
        target_link_libraries(mainlocal.elf PRIVATE
        /home/goncalo/buildroots/buildroot-2021.08-Fato/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib/libpython3.9.so
        -lpthread
        -lrt
        )

        #Daemon Configuration

        set(DSOURCES
        daemon/button_sys.cpp
        daemon/door_sys.cpp
        daemon/motion_sys.cpp
        )

        # Daemon process
        add_executable(daemon.elf daemon/maind.cpp daemon/Cdaemon.cpp daemon/data_errors.cpp ${DSOURCES}) #${daemon_SRCS}

        target_include_directories(daemon.elf PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/daemon
        )

        #endif()

        #
        #
        #

        if(FALSE)

        ##NORMAL COMPILE VERSION

        cmake_minimum_required(VERSION 3.22.1)
        project(CMS_test)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)

        # Enable debugging symbols
        set(CMAKE_BUILD_TYPE Debug)

        # Add compiler flags for debugging
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

        # Set the PYTHONPATH in CMakeLists.txt
        set(ENV{PYTHONPATH} "/usr/local/lib/python3.10/dist-packages:$ENV{PYTHONPATH}")

        # Packages
        find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
        include_directories(${Python3_INCLUDE_DIRS})

        # Executable directory
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

        # Including all include directories
        include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/main/inc
        # Add other include directories if needed
        )

        set(SOURCES
        main/main.cpp
        main/src/house_system.cpp
        main/src/database_sys.cpp
        main/src/relay_sys.cpp
        )

        add_executable(test ${SOURCES})

        # Link against Python library
        target_link_libraries(test PRIVATE Python3::Python)

        # Specify include directories for the target
        target_include_directories(test PRIVATE
        ${Python3_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/main/inc
        # Add other include directories if needed
        )

        #Daemon Configuration

        set(DSOURCES
        daemon/button_sys.cpp
        daemon/door_sys.cpp
        daemon/motion_sys.cpp
        )

        # Daemon process
        add_executable(daemon.o daemon/maind.cpp daemon/Cdaemon.cpp daemon/data_errors.cpp ${DSOURCES}) #${daemon_SRCS}

        target_include_directories(daemon.o PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/daemon
        )

        


        endif()

        #To execute debug sudo: -E gdb --args ./test
        #To execute this program: sudo -E ./main.o 