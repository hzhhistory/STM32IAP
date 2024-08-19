cmake_minimum_required(VERSION 3.0.0)

project(SU17IAR
    LANGUAGES C ASM
)
# SET(CMAKE_C_FLAGS_DEBUG "-Og -g -ggdb3" CACHE INTERNAL "c debug compiler flags")
file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(CMAKE_BUILD_TYPE Debug)

set(DEF " ")

file(GLOB LINKER_SCRIPT_BOOT ${CMAKE_CURRENT_SOURCE_DIR}/link/${CHIPTYPE}_BOOT.ld)
file(GLOB LINKER_SCRIPT_APP ${CMAKE_CURRENT_SOURCE_DIR}/link/${CHIPTYPE}_APP.ld)

if(TAR_PRO STREQUAL "BOOT")
    set(target_name "${PROJECT_NAME}_boot")
    set(LINKER_SCRIPT "${LINKER_SCRIPT_BOOT}")
    list(APPEND DEF "-DBOOT")
    set(target_project "main_boot.c")
elseif(TAR_PRO STREQUAL "APP")
    set(target_name "${PROJECT_NAME}_app")
    set(LINKER_SCRIPT "${LINKER_SCRIPT_APP}")
    list(APPEND DEF "-DAPP")
    set(target_project "main_app.c")

endif()
set(CMAKE_EXE_LINKER_FLAGS "--specs=nano.specs -T${LINKER_SCRIPT} -Wl,-Map=${target_name}.map,--cref -Wl,--gc-sections")

add_definitions(
    ${DEF}
)

add_subdirectory(stm32-libs/src)
add_subdirectory(stm32IAP/src)
# add_subdirectory(Src/SEGGER_RTT)

include_directories(Src/delay Src/LM3644YFFR Src/port Src/port Src/SEGGER_RTT/RTT Src/SEGGER_RTT/Config)

file(GLOB_RECURSE SOURCES "Src/delay/*.*" "Src/LM3644YFFR/*.*" "Src/port/*.*"  "Src/main.c" "Src/SEGGER_RTT/RTT/*.*" "Src/SEGGER_RTT/Config/*.*")

add_executable(${target_name}.elf
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/${target_project}
    ${SOURCES}

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
