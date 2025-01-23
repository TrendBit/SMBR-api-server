# SMBR-api-server
Server providing an API for controlling SMBR and retrieving data.


## Project Status
This project is actively developed in the `dev` branch. The `main` branch contains the stable release version. Changes from dev are periodically merged into `main`. You can see the version changes in `Changelog.md`.


## Requirements
- CMake 3.20 or later
- OATPP 1.4.0
- OATPP-Swagger 1.4.0
- SocketCAN
- Boost Asio

## Installing Dependencies
1. Install CMake:
    ```sh
    sudo apt install cmake
    ```
2. Install OATPP:
    ```sh
    git clone https://github.com/oatpp/oatpp
    cd oatpp
    mkdir build && cd build
    cmake ..
    make install
    ```
3. Install OATPP-Swagger:
    ```sh
    git clone https://github.com/oatpp/oatpp-swagger
    cd oatpp-swagger
    mkdir build && cd build
    cmake ..
    make install
    ```
4. Install SocketCAN:
    ```sh
    sudo apt install can-utils
    ```
5. Install Boost Asio:
    ```sh
    sudo apt-get install libboost-all-dev
    ```

## Building the Project
1. Clone this repository:
    ```sh
    git clone https://github.com/TrendBit/SMBR-api-server.git
    cd SMBR-api-server
    git submodule update --init --recursive
    ```
2. Create a build directory and compile the project:
    ```sh
    mkdir -p build && cd build
    cmake ..
    make
    sudo make install
    ```
Then start with `api-server` or install as service.

## Installing and Managing the Service
```bash  
sudo make install-service  
# Restart daemon to load new service description  
sudo systemctl daemon-reload  
# Enable service startup after next reboot  
sudo systemctl enable api-server  
# Start service now  
sudo systemctl start api-server
# Check if service is running  
sudo systemctl status api-server  
```  

## Running the Application
1. After a successful compilation, run the server:
    ```sh
    ./api-server
    ```

## Verifying Functionality
1. Verify that the application is running on the correct port (default is 8089):
    Open a web browser on the device you are connected to via SSH and go to URL: `http://ip_address_of_rpi:8089/swagger/ui` and test endpoints.



## Project Diagram
```
+--------------------+         +----------------+         +------------------+      +-------------------------+
|     Swagger UI     |         |  oatpp-server  |         |      gate        |      | Hardware (Raspberry Pi) |
+--------------------+         +----------------+         +------------------+      +-------------------------+
           |                             |                         |                            |
           |                             |                         |                            |
           |----1.HTTP Request --------> |                         |                            |
           |                             |                         |                            |
           |                             |----2.Controller ----->  |                            |
           |                             |    (call API methods)   |                            |
           |                             |                         |                            |
           |                             |                         |                            |
           |                             |                         |----3.Send CAN Message ---> |
           |                             |                         |                            |
           |                             |                         |<---4.CAN Response -------- |
           |                             |                         |                            |
           |                             |<---5.Return Response -- |                            |
           |<---6.HTTP Response ---------|                         |                            |
           |                             |                         |                            |
```


