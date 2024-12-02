# Changelog SMBR Api Server
The version number consists of MAJOR.MINOR identifiers. The MINOR version changes correspond to the addition of new API endpoints.

# 0.6 Restart
- Added endpoint {module}/restart
- Resets the module and starts main application firmware again
- added new method for data filling - ping, module request, core temp

# 0.5 LED
- Added endpoint control/set-led-intensity
- Sets the selected channel of LED panel to given intensity.
- For ping added automatic incrementation by 1
- Server changed to listen on all network interfaces (0.0.0.0)
- Added script to test effective speed using ping

# 0.4 Core temp
- Added endpoint {module}/core_temp
- Returns the current temperature of CPU/MCU core values of the computing unit.

# 0.3 Ping
- Added endpoint {module}/ping
- Sends ping request to target module and wait for response. 
- Prints the time it takes for the module to respond to a ping.

# 0.2 Modules request
- Added endpoint system/modules 
- Returns a list of all modules that have responded to the identification message and can therefore be considered available on the device.

# 0.1 Initial concept
- Provides the foundational architecture for asynchronous communication over CAN
- Prepared for adding API endpoints
- No fully functional endpoints implemented yet