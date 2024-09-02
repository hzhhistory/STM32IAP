add_library(stm32Libs)

message(STATUS "load startup file : startup_${STARTUPTYPE}.s")

file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CHIPLIBDIR}/src/*.c)

if(NOT CUSTOM_STM32_SYSFILE_PATH)
    set(CUSTOM_STM32_SYSFILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CHIPLIBDIR}/sys)
    file(GLOB SYSSRC ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CHIPLIBDIR}/sys/*.c)
else()
    file(GLOB SYSSRC ${CUSTOM_STM32_SYSFILE_PATH}/*.c)
endif()

# 移除冲突项
list(REMOVE_ITEM LIBSRC "${CMAKE_CURRENT_SOURCE_DIR}/libs/${CHIPLIBDIR}/src/${REMOVELIBSRC}")

target_sources(stm32Libs
    PRIVATE
    ${SYSSRC}
    ${LIBSRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/${CHIPLIBDIR}/startup_${STARTUPTYPE}.s
)

target_include_directories(stm32Libs
    PUBLIC
    ${CUSTOM_STM32_SYSFILE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CHIPLIBDIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ARM-CMSIS/Include
    "${PROJECT_BINARY_DIR}"
)

