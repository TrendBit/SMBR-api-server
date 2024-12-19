cmake_minimum_required(VERSION 3.20)
project(client_server_project)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)

# --- GateLibrary (gate) ---

add_library(GateLibrary
  gate/base/CommonModule.cpp
  gate/core/CoreModule.cpp
  gate/control/ControlModule.cpp
  gate/sensor/SensorModule.cpp
  gate/system/SystemModule.cpp
  gate/can/CanIdGenerator.cpp
  gate/can/CanMessage.cpp
  gate/can/CanBus.cpp
  gate/can/CanRequest.cpp
  gate/can/CanRequestManager.cpp
)

target_include_directories(GateLibrary
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gate>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server/dto>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    /usr/local/include/oatpp-1.4.0
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/gate/codes/messages
)

target_link_libraries(GateLibrary
  PUBLIC 
        oatpp::oatpp
        spdlog::spdlog
)

install(
  TARGETS GateLibrary
  EXPORT GateLibraryTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/GateLibrary"
)

install(
  FILES gate/*.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GateLibrary
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/GateLibraryConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GateLibrary
)

install(
  EXPORT GateLibraryTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GateLibrary
)


# --- ServerLibrary (oatpp-server) ---

find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-swagger 1.4.0 REQUIRED)

add_library(ServerLibrary
  oatpp-server/controller/MyController.cpp
  oatpp-server/AppComponent.hpp
)

target_include_directories(ServerLibrary
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server
    ${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server/dto
    ${CMAKE_CURRENT_SOURCE_DIR}/gate
    /usr/local/include/oatpp-1.4.0
    ${CMAKE_CURRENT_SOURCE_DIR}/gate/codes/messages
)

target_link_libraries(ServerLibrary
  PUBLIC oatpp::oatpp
  PUBLIC oatpp::oatpp-swagger
  PUBLIC GateLibrary
)

add_executable(Server oatpp-server/App.cpp)

target_link_libraries(Server ServerLibrary)

add_dependencies(Server ServerLibrary)

set_target_properties(ServerLibrary Server PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
  CXX_STANDARD_REQUIRED ON
)

add_definitions(
  -DOATPP_SWAGGER_RES_PATH="/usr/local/include/oatpp-1.4.0/bin/oatpp-swagger/res"
)