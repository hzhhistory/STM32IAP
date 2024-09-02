set(CHIPTYPE "STM32F301K8U6")
set(STARTUPTYPE "stm32f30x")
set(CHIPLIBDIR "stm32f30")
set(BUILD_FLAG SLAVE)
message(STATUS "Chip type : STM32F301K8U6")   

if(LIBTYPE STREQUAL "STD")
    add_definitions(
        -DARM_CM4        
        -DSTM32
        -DSTM32F302x8
        -DUSE_STDPERIPH_DRIVER 
    ) 
    message(STATUS "Using stdPeriph libs")   
endif()

include(${CMAKE_CURRENT_LIST_DIR}/arm-none-eabi.cmake)
