cmake_minimum_required(VERSION 3.20)
project(client_server_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# --- QtOpenAPIClient (gate) ---

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Gui)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(QtOpenAPIClient
  gate/base/CommonModule.cpp
  gate/core/CoreModule.cpp
  gate/control/ControlModule.cpp
  gate/sensor/SensorModule.cpp
  gate/system/SystemModule.cpp
  gate/can/CanHandler.cpp
)

target_include_directories(QtOpenAPIClient
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gate>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(QtOpenAPIClient
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Gui
)

install(
    TARGETS QtOpenAPIClient
    EXPORT QtOpenAPIClientTargets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/QtOpenAPIClient"
)

install(
    FILES gate/*.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtOpenAPIClient
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/QtOpenAPIClientConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtOpenAPIClient
)

install(
    EXPORT QtOpenAPIClientTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtOpenAPIClient
)


# --- ServerLibrary (oatpp-server) ---

find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-swagger 1.4.0 REQUIRED)

add_library(ServerLibrary
    oatpp-server/controller/MyController.cpp
    oatpp-server/dto/MyMessageDto.hpp
    oatpp-server/AppComponent.hpp
)

target_include_directories(ServerLibrary
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/base
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/control
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/sensor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/system
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gate/can
)

target_link_libraries(ServerLibrary
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-swagger
    PUBLIC QtOpenAPIClient
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
