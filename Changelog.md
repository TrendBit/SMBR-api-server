# Changelog SMBR Api Server
The version number consists of MAJOR.MINOR identifiers. The MINOR version changes correspond to the addition of new API endpoints.

# 0.12 Aerator
- Added endpoint GET and POST /control/aerator/speed
- Added endpoint GET and POST /control/aerator/flowrate
- Added endpoint /control/aerator/move
- Added endpoint /control/aerator/stop

# 0.11 Cuvette pump
- Added endpoint GET and POST /control/cuvette_pump/speed
- Added endpoint GET and POST /control/cuvette_pump/flowrate
- Added endpoint /control/cuvette_pump/move
- Added endpoint /control/cuvette_pump/prime
- Added endpoint /control/cuvette_pump/purge
- Added endpoint /control/cuvette_pump/stop
- Added script for testing throughput
- Refactoring temperature control endpoints into retrieval functions to handle values below -30 for error reporting

# 0.10 Heater
- Added endpoint GET and POST /control/heater/intensity
- Added endpoint GET and POST /control/heater/target_temperature
- Added endpoint GET /control/heater/plate_temperature
- Added endpoint POST /control/heater/turn_off
- Fixing API crashes in CanRequestManager by handling them with mutexes
- Replaced shared_ptr with unique_ptr where possible

# 0.9 Load
- Added endpoint /{module}/load 
- Gets module CPU/MCU load
- Updating CMakeLists for new version of Codes

# 0.8 CAN Bootloader
- Added endpoint {module}/can_bootloader
- Resets the module and put it into CAN bus bootloader mode so new firmware can be flashed over CAN bus from RPi
- Added and integrated new method for sending CAN messages via the can bus without expecting a response
- Added support for using uid with and without 0x prefix for restart, usb_bootloader and can_bootloader endpoints
- Added script to test effective speed using ping in C++

# 0.7 USB Bootloader
- Added endpoint {module}/usb_bootloader
- Resets the module and put it into USB bootloader mode so new firmware can be flashed via USB-C connector on board

# 0.6 Restart
- Added endpoint {module}/restart
- Resets the module and starts main application firmware again
- Added new method for data filling - ping, module request, core temp

# 0.5.1 LED
- Endpoint control/set-led-intensity rename to /control/led_intensity/{channel}
- Added endpoint POST /control/led_intensity - sets all channels of LED panel to given intensity
- Added endpoint GET /control/led_intensity/{channel} - retrieves current intensity of selected channel of LED panel
- Added endpoint /control/led_temperature - retrieves temperature of LED panel

# 0.5 LED
- Added endpoint control/set-led-intensity
- Sets the selected channel of LED panel to given intensity
- For ping added automatic incrementation by 1
- Server changed to listen on all network interfaces (0.0.0.0)
- Added script to test effective speed using ping in python

# 0.4 Core temp
- Added endpoint {module}/core_temp
- Returns the current temperature of CPU/MCU core values of the computing unit

# 0.3 Ping
- Added endpoint {module}/ping
- Sends ping request to target module and wait for response
- Prints the time it takes for the module to respond to a ping.

# 0.2 Modules request
- Added endpoint system/modules 
- Returns a list of all modules that have responded to the identification message and can therefore be considered available on the device

# 0.1 Initial concept
- Provides the foundational architecture for asynchronous communication over CAN
- Prepared for adding API endpoints
- No fully functional endpoints implemented yet
