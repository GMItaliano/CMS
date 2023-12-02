        if(FALSE)
        cmake_minimum_required(VERSION 3.22.1)
        project(CMS_test)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)


        # Packages
        #find_package(Python3 COMPONENTS Development)
        find_package (Python3 REQUIRED COMPONENTS Interpreter Development)
        include_directories(${Python3_INCLUDE_DIRS})

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

        set(SOURCES
        main/main.cpp
        main/src/house_system.cpp
        main/src/database_sys.cpp
        )

        add_executable(main.o ${SOURCES})

        # Main process
        #add_executable(main.o 
        #   main/main.cpp 
        #    main/src/house_system.cpp 
        #    main/src/databse_sys.cpp) #${main_SRCS}

        # Link against Python library
        #target_link_libraries(main.o PRIVATE Python3::Python)

        # All source files for daemon
        #file(GLOB daemon_SRCS
        #    "${CMAKE_CURRENT_SOURCE_DIR}/daemon/daemon.cpp"
        #)

        # Daemon process
        add_executable(daemon.o daemon/maind.cpp daemon/Cdaemon.cpp) #${daemon_SRCS}

        # Link against Python library
        #target_link_libraries(daemon.o ${Python3_LIBRARIES})


        target_include_directories(main.o PRIVATE
                ${Python3_INCLUDE_DIRS})

        target_link_directories(main.o PRIVATE
                ${Python3_LIBRARY_DIRS})

        target_link_libraries(main.o PRIVATE
                ${Python3_LIBRARIES})
        endif()
        #
        #
        #

        if(FALSE)
        cmake_minimum_required(VERSION 3.22.1)
        project(CMS_test)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        
        # Enable debugging symbols
        set(CMAKE_BUILD_TYPE Debug)
        
        # Add compiler flags for debugging
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
        
        # Packages
        find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
        include_directories(${Python3_INCLUDE_DIRS})

        # Find Firebase Admin
        find_package(FIREBASE_ADMIN_LIBRARY firebase-admin PATHS /home/goncalo/.local/lib/python3.10/site-packages)

        
        # Executable directory
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        
        # Including all include directories
        include_directories(
            ${CMAKE_CURRENT_SOURCE_DIR}/main/inc
            ${CMAKE_CURRENT_SOURCE_DIR}/../inc
            ${CMAKE_CURRENT_SOURCE_DIR}/daemon/
        )
        
        set(SOURCES
            main/main.cpp
            main/src/house_system.cpp
            main/src/database_sys.cpp
        )
        
        add_executable(main.o ${SOURCES})
        
        # Link against Python libraries
        target_link_libraries(main.o PRIVATE ${Python3_LIBRARIES} ${FIREBASE_ADMIN_LIBRARY}) 
        
        # Include Python directories
        target_include_directories(main.o PRIVATE ${Python3_INCLUDE_DIRS})
        endif()

        #
        #
        #

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

        # Daemon process
        add_executable(daemon.o daemon/maind.cpp daemon/Cdaemon.cpp) #${daemon_SRCS}

        #To execute this program: sudo -E ./main.o 


        