add_library(stm32iap)

file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

target_sources(stm32iap
    PRIVATE
    ${LIBSRC}
)

target_include_directories(stm32iap
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

