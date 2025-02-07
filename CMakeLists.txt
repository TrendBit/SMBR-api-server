cmake_minimum_required(VERSION 3.20)
project(client_server_project)

set(CMAKE_CXX_STANDARD 17)

find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-swagger 1.4.0 REQUIRED)
find_package(spdlog REQUIRED)

add_executable(api-server
  oatpp-server/App.cpp
  oatpp-server/controller/Controller.cpp
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
  backward/backward.cpp
)

target_include_directories(api-server
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server
    ${CMAKE_CURRENT_SOURCE_DIR}/oatpp-server/dto
    ${CMAKE_CURRENT_SOURCE_DIR}/gate
    /usr/local/include/oatpp-1.4.0
    ${CMAKE_CURRENT_SOURCE_DIR}/gate/codes/messages
    ${CMAKE_CURRENT_SOURCE_DIR}/gate/can
    ${CMAKE_CURRENT_SOURCE_DIR}/backward
)

target_link_libraries(api-server
  PRIVATE oatpp::oatpp
  PRIVATE oatpp::oatpp-swagger
  PRIVATE spdlog::spdlog
)

set_target_properties(api-server PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
  CXX_STANDARD_REQUIRED ON
)

add_definitions(
  -DOATPP_SWAGGER_RES_PATH="/usr/local/include/oatpp-1.4.0/bin/oatpp-swagger/res"
)

install(TARGETS api-server
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api-server.service
  DESTINATION /etc/systemd/system
)

add_custom_target(install-service
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_SOURCE_DIR}/api-server.service
          /etc/systemd/system/api-server.service
  DEPENDS api-server
)
