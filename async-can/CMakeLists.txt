cmake_minimum_required(VERSION 3.10)
project(AsyncCanRequestManager)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(AsyncCanRequestManager
    src/main.cpp
    src/CanBus.cpp
    src/CanMessage.cpp
    src/CanRequest.cpp
    src/CanRequestManager.cpp
)

target_link_libraries(AsyncCanRequestManager ${Boost_LIBRARIES} rt)
