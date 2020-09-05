SET(HOLLYWOOD_TEST hollywood-test)

INCLUDE_DIRECTORIES("/usr/local/include")
LINK_DIRECTORIES("/usr/local/lib")

ADD_EXECUTABLE(${HOLLYWOOD_TEST} tests.cpp
                                 tests/actor.cpp
                                 tests/channel.cpp)
TARGET_LINK_LIBRARIES(${HOLLYWOOD_TEST} gtest)

INSTALL(TARGETS ${HOLLYWOOD_TEST}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        PERMISSIONS OWNER_READ
                    OWNER_WRITE
                    OWNER_EXECUTE
                    GROUP_READ
                    GROUP_EXECUTE
                    WORLD_READ
                    WORLD_EXECUTE)

ADD_TEST(${HOLLYWOOD_TEST} ${HOLLYWOOD_TEST})
