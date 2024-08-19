cmake_minimum_required(VERSION 3.0.0)

project(stm32iap_example
    LANGUAGES C ASM
)

file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)


set(DEF " ")

file(GLOB LINKER_SCRIPT_BOOT ${CMAKE_CURRENT_SOURCE_DIR}/example_build/link/${CHIPTYPE}_BOOT.ld)
file(GLOB LINKER_SCRIPT_APP ${CMAKE_CURRENT_SOURCE_DIR}/example_build/link/${CHIPTYPE}_APP.ld)

if(TAR_PRO STREQUAL "BOOT")
    set(target_name "${PROJECT_NAME}_boot")
    set(LINKER_SCRIPT "${LINKER_SCRIPT_BOOT}")
    list(APPEND DEF "-DBOOT")
elseif(TAR_PRO STREQUAL "APP")
    set(target_name "${PROJECT_NAME}_app")
    set(LINKER_SCRIPT "${LINKER_SCRIPT_APP}")
    list(APPEND DEF "-DAPP")
endif()
set(CMAKE_EXE_LINKER_FLAGS "--specs=nano.specs -T${LINKER_SCRIPT} -Wl,-Map=${target_name}.map,--cref -Wl,--gc-sections")

add_definitions(
    ${DEF}
)

add_subdirectory(example_build/chiplib/)
add_subdirectory(src)

add_executable(${target_name}.elf
    ${CMAKE_CURRENT_SOURCE_DIR}/example_build/example_build.c
)

target_link_libraries(${target_name}.elf
    stm32Libs
    stm32iap
)

target_include_directories(${target_name}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${PROJECT_BINARY_DIR}"
)

set(ELF_FILE ${PROJECT_BINARY_DIR}/${target_name}.elf)
set(HEX_FILE ${PROJECT_BINARY_DIR}/${target_name}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${target_name}.bin)

add_custom_command(TARGET "${target_name}.elf" POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ELF_FILE} ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Oihex  ${ELF_FILE} ${HEX_FILE}
)
