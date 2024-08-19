set(CHIPTYPE "STM32F030F4P6")
set(STARTUPTYPE "stm32f030")
set(CHIPLIBDIR "stm32f0")
set(BUILD_FLAG SLAVE)
message(STATUS "Chip type : STM32F030F4P6")   

if(LIBTYPE STREQUAL "STD")
    add_definitions(
        -DARM_CM0        
        -DSTM32
        -DSTM32F030
        -DUSE_STDPERIPH_DRIVER 
    ) 
    message(STATUS "Using stdPeriph libs")   
endif()

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cortex-m0.cmake)