add_library(serial)

file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

target_sources(serial
    PRIVATE
    ${LIBSRC}
)
target_link_libraries(serial
      stm32Libs
)
target_include_directories(serial
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

