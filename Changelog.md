# Changelog SMBR Api Server
The version number consists of MAJOR.MINOR identifiers. The MINOR version changes correspond to the addition of new API endpoints.

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