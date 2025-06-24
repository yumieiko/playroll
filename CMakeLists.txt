cmake_minimum_required(VERSION 3.31)
project(windows_pwe)

include(FetchContent)
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)
include_directories(yaml-cpp::yaml-cpp)
set(CMAKE_CXX_STANDARD 20)

add_executable(windows_pwe src/main.cpp
        src/base64/base64.cpp
        src/pwe_parser/PWEParser.cpp
        src/pwe_parser/PWEParser.h)
target_link_libraries(windows_pwe PUBLIC yaml-cpp::yaml-cpp)
