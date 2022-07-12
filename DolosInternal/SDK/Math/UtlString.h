#pragma once

#ifndef UTL_STRING_H
#define UTL_STRING_H

#include "UtlMemory.h"

struct UtlString {
    UtlMemory<char> utlBuffer;
    int iLength;

    const char* data() const noexcept { return utlBuffer.tMemory; }
};

#endif // !UTL_STRING_H
