#pragma once
#ifndef UTL_VECTOR_H
#define UTL_VECTOR_H


template <typename T>
class UtlVector {
public:
    constexpr T& operator[](int i) noexcept { return pMemory[i]; }
    constexpr const T& operator[](int i) const noexcept { return pMemory[i]; }

    T* pMemory;
    int iAllocateCount;
    int iGrowSize;
    int iSize;
    T* pElements;
};


#endif // !UTL_VECTOR_H
