#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include <boost/asio.hpp>
#include "interceptors/MyResponseInterceptor.hpp"

/**
 * @class AppComponent
 * @brief Class that creates and holds application components and registers them in the Oat++ environment.
 * 
 * The order of components initialization is from top to bottom.
 */
class AppComponent {
public:

  /**
   * @brief Function to get the local IP address.
   * 
   * This function attempts to determine the local IP address by resolving the IP of a remote host.
   * 
   * @return The local IP address as a string. Returns "0.0.0.0" in case of failure.
   */
  /*
  std::string getLocalIPAddress() {
    try {
      boost::asio::io_service io_service;
      boost::asio::ip::udp::resolver resolver(io_service);
      boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "google.com", "");
      boost::asio::ip::udp::resolver::iterator endpoints = resolver.resolve(query);
      boost::asio::ip::udp::endpoint endpoint = *endpoints;
      boost::asio::ip::udp::socket socket(io_service);
      socket.connect(endpoint);
      boost::asio::ip::address addr = socket.local_endpoint().address();
      return addr.to_string();
    } catch (std::exception &e) {
      return "0.0.0.0"; 
    }
  }
  */
  
  /**
   * @brief Create ConnectionProvider component which listens on a specific port.
   * 
   * This component creates a TCP connection provider bound to the local IP address determined by `getLocalIPAddress()`.
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
    //std::string serverIp = getLocalIPAddress();
    return oatpp::network::tcp::server::ConnectionProvider::createShared({/*serverIp.c_str()*/"0.0.0.0", 8089, oatpp::network::Address::IP_4});
  }());
  
  /**
   * @brief Create Router component.
   * 
   * The router is responsible for routing HTTP requests to the appropriate handler based on the request path and method.
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   * @brief Create ConnectionHandler component which uses the Router component to route requests.
   * 
   * This component handles incoming connections and delegates the requests to the appropriate route.
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); 
  
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    return connectionHandler;
  }());

  /**
   * @brief Create ObjectMapper component to serialize/deserialize DTOs in Controller's API.
   * 
   * This component is responsible for converting between JSON and C++ objects.
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {

    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;

    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;

  }());

  /**
   * @brief General API documentation info for Swagger UI.
   * 
   * This component provides metadata about the API, such as the title, description, version, and license.
   */
  OATPP_CREATE_COMPONENT(
    std::shared_ptr<oatpp::swagger::DocumentInfo>, 
    swaggerDocumentInfo
  )([] {
    
    oatpp::swagger::DocumentInfo::Builder builder;
    builder
      .setTitle("Smart Modular BioReactor API/CAN Specification")
      .setDescription("API facilitating communication with SMBR (Smart Modular BioReactor), which is used for algae cultivation.")
      .setVersion("0.1.0")
      .setContactName("Your Name")
      .setLicenseName("Apache License, Version 2.0")
      .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0");
    return builder.build();
  }());

  /**
   * @brief Swagger-UI Resources.
   * 
   * This component loads the resources required by the Swagger UI.
   */
  OATPP_CREATE_COMPONENT(
    std::shared_ptr<oatpp::swagger::Resources>, 
    swaggerResources
  )([] {
    return oatpp::swagger::Resources::loadResources(
      OATPP_SWAGGER_RES_PATH
    );
  }());

};