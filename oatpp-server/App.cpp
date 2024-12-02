#include "./controller/MyController.hpp"
#include "./AppComponent.hpp"
#include "can/CanBus.hpp"
#include "can/CanRequestManager.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp-swagger/Controller.hpp"

#include <iostream>
#include <boost/asio.hpp>
#include <thread>

/**
 * @brief Run the server and manage the CanBus and CanRequestManager.
 * 
 * This function sets up the server, initializes the CAN bus communication, and runs the server loop.
 * 
 * @param io_context Reference to Boost ASIO io_context for asynchronous operations.
 */
void run(boost::asio::io_context& io_context) {

  AppComponent components;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, contentMappers);

  CanBus canBus(io_context);
  CanRequestManager canRequestManager(io_context, canBus);

  SystemModule& systemModule = SystemModule::getInstance(io_context, canRequestManager);
  CommonModule commonModule(io_context, canRequestManager);

  ControlModule controlModule(io_context, canRequestManager); 

  auto myController = std::make_shared<MyController>(
      contentMappers, io_context, systemModule, commonModule, controlModule, canRequestManager
  );  
  router->addController(myController);

  auto docEndpoints = myController->getEndpoints();
  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  router->addController(swaggerController);

  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  oatpp::network::Server server(connectionProvider, connectionHandler);

  OATPP_LOGi("MyApp", "Server running on port {}", connectionProvider->getProperty("port").toString()->c_str());

  server.run();
}


/**
 * @brief Main entry point of the application.
 * 
 * Initializes the Oat++ environment, starts the server, and runs the CAN bus communication loop.
 * 
 * @return int Exit code.
 */
int main(int argc, const char * argv[]) {

  oatpp::Environment::init();

  boost::asio::io_context io_context;

  auto work = std::make_unique<boost::asio::io_context::work>(io_context);

  std::thread io_thread([&io_context](){
    io_context.run();
  });

  run(io_context);
  
  io_thread.join();

  work.reset();
  oatpp::Environment::destroy();
  
  return 0;
}

