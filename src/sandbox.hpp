#ifndef sandbox_hpp
#define sandbox_hpp

#include "testingPage.hpp"

class Sandbox : public core::Window
{
public:
    
    Sandbox(std::string title);
};

core::Window *core::createWindow();


#endif