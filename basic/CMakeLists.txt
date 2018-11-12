set(PROJECT_NAME "libqss-basic")

set(SOURCE
    main.cc
    )

add_executable(${PROJECT_NAME}
        ${SOURCE})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core Qt5::Network QtShadowsocks)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/lib)

