#pragma once

#include "IApplication.h"
#include "ImportExportAppFromExecutable.h"
#include "EntryPoint/Linux/Entry.h"

class Application
{
public:

    Application() = default;
    ~Application() = default;

public:

    void Initialize();

    void Update();

    void Destroy();

};

