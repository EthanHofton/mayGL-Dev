#include "sandbox.hpp"

Sandbox::Sandbox(std::string title) : core::Window(title)
{
    new TestingPage(this, "testingPage");
    
    addPage("testingPage");
    core::Graphics::instance()->relativeMouse(true);
    core::Graphics::instance()->clearColor(glm::vec4(0,0,0,1));
}

core::Window *core::createWindow()
{
    return new Sandbox("maycraft");
}