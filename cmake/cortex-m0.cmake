# Name of the target
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0)

set(MCPU_FLAGS "-mcpu=cortex-m0 -mthumb")
set(VFP_FLAGS "-Wall -fdata-sections -ffunction-sections")
set(SPEC_FLAGS "--specs=nosys.specs")