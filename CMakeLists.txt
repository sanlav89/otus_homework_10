cmake_minimum_required(VERSION 3.5)

# PROJECT
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(PROJECT_NAME "join_server" CACHE STRING "Project name")
project(${PROJECT_NAME} VERSION ${PROJECT_VESRION})

# Packages
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIR})

# SOURCES
set (
    SOURCES
        server.cpp
        session.cpp
        db.cpp
        main.cpp
)

# HEADERS
 set (
     HEADERS
        server.h
        session.h
        db.h
 )

# EXECUTABLE
add_executable(join_server ${SOURCES} ${HEADERS})
add_executable(join_client join_client.cpp)
add_executable(gtest_query gtest_query.cpp db.cpp)

# PROPERTIES
set_target_properties (
    join_server
    join_client
    gtest_query
        PROPERTIES
            CMAKE_CXX_STANDARD 17
            CMAKE_CXX_STANDARD_REQUIRED ON
)

# Linking
target_link_libraries(
    join_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(
    join_client
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)

# Compiler options
if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(join_client PRIVATE
        /W4
    )
    target_compile_options(gtest_query PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(join_client PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(gtest_query PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

### Package
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

### UNIT-tests
# GoogleTest package
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Tests linking
gtest_discover_tests(gtest_query)
target_link_libraries(
    gtest_query
    gtest_main
)

# Testing
enable_testing()
add_test(gtest_query gtest_query)

