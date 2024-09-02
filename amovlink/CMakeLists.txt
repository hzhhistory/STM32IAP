cmake_minimum_required(VERSION 3.0.0)

project(amovLink_example
    LANGUAGES C
)

add_subdirectory(src)

# set(CMAKE_EXE_LINKER_FLAGS "-Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/example_build/example.lds,-Map=gcc.map")

add_executable(amovLink_example
    example_build/amovlink.c
)

target_link_libraries(amovLink_example
    amovlink
)

target_include_directories(amovLink_example 
    PUBLIC
    "${PROJECT_BINARY_DIR}"
)



