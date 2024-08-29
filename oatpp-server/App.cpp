#include "./controller/MyController.hpp"
#include "./AppComponent.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp-swagger/Controller.hpp"

#include <iostream>
#include <boost/asio.hpp>

void run(boost::asio::io_context& io_context) {

  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Get the ContentMappers component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, contentMappers);

  /* Create MyController and add all of its endpoints to router */
  auto myController = std::make_shared<MyController>(contentMappers, io_context);
  auto myControllerEndpoints = myController->getEndpoints();
  router->addController(myController);

  /* Swagger controller */
  oatpp::web::server::api::Endpoints docEndpoints;
  docEndpoints.append(myControllerEndpoints);
  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  router->addController(swaggerController);

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGi("MyApp", "Server running on port {}", connectionProvider->getProperty("port").toString()->c_str());

  /* Run server */
  server.run();
}
int main(int argc, const char * argv[]) {

  oatpp::Environment::init();

  boost::asio::io_context io_context;

  auto work = std::make_unique<boost::asio::io_context::work>(io_context);
  std::cout << "Main - Starting io_context.run()" << std::endl;

  std::thread io_thread([&io_context](){
    io_context.run();
    std::cout << "Main - io_context.run() completed" << std::endl;
  });

  /* Run the application */
  run(io_context);
  
  io_thread.join();

  /* Clean-up */
  work.reset();
  oatpp::Environment::destroy();
  
  return 0;
}



