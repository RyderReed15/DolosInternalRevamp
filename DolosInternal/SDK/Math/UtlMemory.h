#pragma once

#ifndef UTL_MEMORY_H
#define UTL_MEMORY_H

template <typename T>
struct UtlMemory {
    T& operator[](int i) const noexcept { return tMemory[i]; }

    T*  tMemory;
    int iAllocationCount;
    int iGrowSize;
};

#endif