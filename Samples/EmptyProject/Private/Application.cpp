#include "Application.h"
#include "Console.h"

DECL_APPLICATION(Application);



// ---------------------------------------------------------------------------------------------------------------------
void Application::Initialize()
{
    Core::WriteToConsole("Initialize!");
}

// ---------------------------------------------------------------------------------------------------------------------
void Application::Update()
{
    // Core::WriteToConsole("Update!");
}

// ---------------------------------------------------------------------------------------------------------------------
void Application::Destroy()
{
    Core::WriteToConsole("Destroy!");
}
