set(LIBNAME "QtShadowsocks")
add_definitions(-DQSS_VERSION="${PROJECT_VERSION}")

set(SOURCE
        QtShadowsocks)

set(HEADERS
        QtShadowsocks)

#include(crypto/CMakeLists.txt)
#include(network/CMakeLists.txt)
#include(types/CMakeLists.txt)
#include(util/CMakeLists.txt)
file(GLOB_RECURSE SOURCE
        crypto/encryptor.cpp
        crypto/chacha.cpp
        crypto/cipher.cpp
        crypto/rc4.cpp

        network/mixedproxy.cpp
        network/socketstream.cpp
        network/tcprelay.cpp
        network/tcprelayclient.cpp
        network/tcprelayserver.cpp
        network/tcpserver.cpp
        network/udprelay.cpp

        net/tcprelay.cc
        net/udprelay.cc
        net/tcphandler.cc

        types/address.cpp
        types/profile.cpp
        types/configuration.cc
        types/server.cc
        types/proxy.cc

        util/addresstester.cpp
        util/common.cpp
        util/controller.cpp
        util/listener.cc)

add_library(${LIBNAME} ${SOURCE})

target_include_directories(${LIBNAME} PRIVATE
        ${BOTAN_INCLUDE_DIRS}
        crypto
        network
        types
        util)

set_target_properties(${LIBNAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(${LIBNAME}
        PUBLIC Qt5::Core
        PUBLIC Qt5::Network
        PRIVATE ${BOTAN_LIBRARY_VAR})

foreach (LIB Qt5Network Qt5Core ${BOTAN_LIBRARIES})
    set(PRIVATE_LIBS "${PRIVATE_LIBS} -l${LIB}")
endforeach ()

configure_file(QtShadowsocks.pc.in ${CMAKE_CURRENT_BINARY_DIR}/QtShadowsocks.pc
        @ONLY
        NEWLINE_STYLE UNIX)

install(TARGETS ${LIBNAME}
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${HEADERS}
        DESTINATION ${INCLUDE_INSTALL_DIR}/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtShadowsocks.pc
        DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
