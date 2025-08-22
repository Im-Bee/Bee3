#pragma once

#include "IApplication.h"


// ---------------------------------------------------------------------------------------------------------------------
#undef DECL_APPLICATION
#define DECL_APPLICATION(x)                             \
    inline void* ::App::ImportApp()                     \
    { return new x(); }                                 \
                                                        \
    inline void ::App::AppInit(void* pApp)              \
    { static_cast<x*>(pApp)->Initialize(); }            \
                                                        \
    inline void ::App::AppUpdate(void* pApp)            \
    { static_cast<x*>(pApp)->Update(); }                \
                                                        \
    inline void ::App::AppDestroy(void* pApp)           \
    { static_cast<x*>(pApp)->Destroy(); }               \
                                                        \
    inline void ::App::AppDelete(void* pApp)            \
    { delete static_cast<x*>(pApp); }                   \



namespace App
{

// ---------------------------------------------------------------------------------------------------------------------
extern inline void* ImportApp();

// ---------------------------------------------------------------------------------------------------------------------
extern inline void AppInit(void*);

// ---------------------------------------------------------------------------------------------------------------------
extern inline void AppUpdate(void*);

// ---------------------------------------------------------------------------------------------------------------------
extern inline void AppDestroy(void*);

// ---------------------------------------------------------------------------------------------------------------------
extern inline void AppDelete(void*);

} // !App

