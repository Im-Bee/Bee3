#pragma once

#include "Exception.h"

#undef ASSERT
#define ASSERT(x, y)                                                          \
    do { if (x != y) {                                                        \
        throw ::Core::Exception("Assertion failed! ('" #x "' != '" #y "')");  \
    } } while (0)                                                             \

