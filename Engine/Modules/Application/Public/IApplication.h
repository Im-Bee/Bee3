#pragma once

namespace App
{

class IApplication
{
public:

    IApplication() = default;
    ~IApplication() = default;

public:

    void Initialize() 
    { throw; }

    void Update() 
    { throw; }

    void Destroy() 
    { throw; }

};

} // !App
