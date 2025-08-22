#pragma once

#include "Console.h"
#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"
#include "EntryPoint/Linux/Entry.h"

class Application
{
public:

    Application() = default;
    ~Application() = default;

public:

    void Initialize() 
    { Core::WriteToConsole("Hello from empty application"); }

    void Update() 
    { Core::WriteToConsole("Updating from empty application"); }

    void Destroy() 
    { Core::WriteToConsole("Destroying from empty application"); }

};

