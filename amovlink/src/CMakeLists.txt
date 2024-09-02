
add_library(amovlink)

file(GLOB LIBSRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

target_sources(amovlink
    PRIVATE
    ${LIBSRC}
)
target_link_libraries(amovlink
RTT
)
target_include_directories(amovlink
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

