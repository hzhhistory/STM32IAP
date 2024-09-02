set(CHIPTYPE "STM32F412RE")
set(STARTUPTYPE "stm32f40_41xxx")
set(CHIPLIBDIR "stm32f4")
message(STATUS "Chip type : STM32F412RE")   

if(LIBTYPE STREQUAL "STD")
    add_definitions(
        -DARM_CM4        
        -DSTM32
        -DSTM32F412xG
        -DUSE_STDPERIPH_DRIVER
    ) 
    set(REMOVELIBSRC "stm32f4xx_fmc.c")
    message(STATUS "Using stdPeriph libs")   
endif()

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cortex-m4.cmake)
