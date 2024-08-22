#ifndef AppComponent_hpp
#define AppComponent_hpp

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
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

  /**
   *  Function to get the local IP address
   */
  
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
  
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
    std::string serverIp = getLocalIPAddress();
    return oatpp::network::tcp::server::ConnectionProvider::createShared({serverIp.c_str(), 8089, oatpp::network::Address::IP_4});
  }());
  
/*
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"127.0.0.1", 8089, oatpp::network::Address::IP_4});
  }());*/

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
  
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    return connectionHandler;
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {

    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;

    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;

  }());

  /**
   *  General API docs info
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
      //.setTermsOfServiceUrl("https://example.com/terms/");
    return builder.build();
  }());

  /**
   *  Swagger-Ui Resources
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

#endif /* AppComponent_hpp */
