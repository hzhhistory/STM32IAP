cmake_minimum_required(VERSION 3.0.0)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}.")

project(example
    LANGUAGES C ASM
)

add_subdirectory(src)

file(GLOB LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/example_build/link/${CHIPTYPE}.ld)
set(CMAKE_EXE_LINKER_FLAGS "--specs=nano.specs -T${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections")

add_executable(${PROJECT_NAME}.elf
    example_build/${PROJECT_NAME}_build.c
)

target_link_libraries(${PROJECT_NAME}.elf
    stm32Libs
)

target_include_directories(${PROJECT_NAME}.elf PUBLIC
    "${PROJECT_BINARY_DIR}"
)

