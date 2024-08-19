# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(BUILD_FLAG SLAVE)

set(MCPU_FLAGS "-mcpu=cortex-m4 -mthumb -march=armv7e-m -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(VFP_FLAGS "-Wall -fdata-sections -ffunction-sections")
set(SPEC_FLAGS "--specs=nosys.specs")