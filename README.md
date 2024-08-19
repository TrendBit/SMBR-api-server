# SMBR-api-server dev
This is the `dev` branch, where active development and testing are taking place. Please note that features in this branch may be incomplete or unstable. The primary purpose of this branch is to integrate and test new features before they are merged into the `main` branch.

**Current focus:** Integration of endpoints into SMBR according to the `api.yaml` documentation.

## Planned Project Structure
```
SMBR-api.server
├── oatpp-server/
│   ├── controller/                       
│   │   ├── MyController.cpp
│   │   └── MyController.hpp
│   ├── dto/
│   │   ├── ModuleListDto.hpp             
│   │   ├── TemperatureDto.hpp           
│   │   ├── PingResponseDto.hpp          
│   │   ├── LoadDto.hpp                  
│   │   ├── CoreTemperatureDto.hpp       
│   │   ├── RestartDto.hpp               
│   │   ├── BootloaderDto.hpp             
│   │   └── PowerSupplyTypeDto.hpp        
│   ├── interceptors/
│   │   └── MyResponseInterceptor.hpp
│   ├── AppComponent.hpp
│   └── App.cpp                      
├──gate
│   ├── base/
│   │   ├── codes.hpp
│   │   ├── CommonModule.cpp   
│   │   ├── CommonModule.hpp
│   │   └── endpoints/
│   │       ├── CommonPingApi.cpp        
│   │       ├── CommonPingApi.hpp        
│   │       ├── CommonLoadApi.cpp        
│   │       ├── CommonLoadApi.hpp        
│   │       ├── CommonCoreTempApi.cpp     
│   │       ├── CommonCoreTempApi.hpp    
│   │       ├── CommonRestartApi.cpp     
│   │       ├── CommonRestartApi.hpp     
│   │       ├── CommonBootloaderApi.cpp  
│   │       └── CommonBootloaderApi.hpp  
│   ├── core/
│   │   ├── CoreModule.cpp               
│   │   └── CoreModule.hpp
│   ├── control/
│   │   ├── ControlModule.cpp             
│   │   └── ControlModule.hpp
│   ├── sensor/
│   │   ├── SensorModule.cpp             
│   │   └── SensorModule.hpp
│   ├── system/
│   │   ├── SystemModule.cpp             
│   │   ├── SystemModule.hpp             
│   │   └── endpoints/
│   │       ├── SystemModulesApi.cpp     
│   │       ├── SystemModulesApi.hpp     
│   │       ├── SystemTemperatureApi.cpp 
│   │       └── SystemTemperatureApi.hpp 
│   ├── can/
│   │   ├── CanMessage.hpp               
│   │   ├── CanMessage.cpp               
│   │   ├── CanHandler.hpp               
│   │   ├── CanHandler.cpp               
│   │   ├── CanInterface.hpp                   
│   │   ├── CanInterface.cpp                   
│   │   ├── CanRequest.hpp               
│   │   ├── CanRequest.cpp               
│   │   ├── CanRequestManager.hpp        
│   │   ├── CanRequestManager.cpp        
│   │   ├── CanError.hpp                 
│   │   └── CanError.cpp                 
│   └── error/
│       ├── ApiError.cpp                 
│       └── ApiError.hpp                                   
└── CMakeLists.txt  
```

