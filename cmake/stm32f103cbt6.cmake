set(CHIPTYPE "STM32F103CBT6")
set(STARTUPTYPE "stm32f10x_hd")
set(CHIPLIBDIR "stm32f1")
set(BUILD_FLAG SLAVE)
message(STATUS "Chip type : STM32F103CBT6")   

if(LIBTYPE STREQUAL "STD")
    add_definitions(
        -DARM_CM3        
        -DSTM32
        -DSTM32F10X_HD
        -DUSE_STDPERIPH_DRIVER 
    ) 
    message(STATUS "Using stdPeriph libs")   
endif()

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cortex-m3.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/iap-config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/amovlink.cmake)