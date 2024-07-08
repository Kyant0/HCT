cmake_minimum_required(VERSION 3.22.1)

project(com.kyant.hct)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fvisibility=hidden -flto -fdata-sections -ffunction-sections -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fvisibility=hidden -flto -fdata-sections -ffunction-sections -fomit-frame-pointer")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,-s")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

add_library(${CMAKE_PROJECT_NAME} SHARED
        lib.cpp
        ciexyz.cpp
        srgb.cpp
        achromatic_cielab.cpp
        zcam.cpp
        kcam.cpp
        cat.cpp
        hct.cpp)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        android
        m)
