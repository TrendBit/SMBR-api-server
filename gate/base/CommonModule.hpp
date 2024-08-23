#ifndef COMMONMODULE_HPP
#define COMMONMODULE_HPP

#include <cstdint>
#include "codes/codes.hpp"

class CommonModule {
public:
    CommonModule(Codes::Module mod, Codes::Instance inst);
    virtual ~CommonModule();
    float ping();

protected:
    Codes::Module module;
    Codes::Instance instance;
};

#endif // COMMONMODULE_HPP
