project(${PROJECT_NAME}-test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include GoogleTest ==========================================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()
FetchContent_MakeAvailable(googletest)

include_directories(../src)
include(GoogleTest)

add_executable(gameTest gameTest.cpp)
target_link_libraries(gameTest PUBLIC gtest gtest_main ${PROJECT_BASE_NAME}-src-lib ${REQUIRED_LIBS_QUALIFIED})
gtest_discover_tests(gameTest)

