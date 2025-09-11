#pragma once

namespace App
{

class IApplication
{
public:

    IApplication() = delete;
    ~IApplication() = delete;

public:

    void Initialize() 
    { throw; }

    void Update() 
    { throw; }

    void Destroy() 
    { throw; }

};

} // !App
