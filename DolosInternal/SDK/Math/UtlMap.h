#pragma once

#ifndef UTL_MAP_H
#define UTL_MAP_H

#include "UtlMemory.h"

template <typename Key, typename Value>
struct Node {
    int iLeftIndex;
    int iRightIndex;
    int iParentIndex;
    int iType;
    Key Key;
    Value Value;
};


template <typename Key, typename Value>
struct UtlMap {
    auto begin() const noexcept { return utlMemory.tMemory; }
    auto end() const noexcept { return utlMemory.tMemory + iNumElements; }

    int FindValue(Key key) const noexcept {
        auto curr = iRootIndex;

        while (curr != -1) {
            const auto el = utlMemory[curr];

            if (el.Key < key)
                curr = el.iRightIndex;
            else if (el.Key > key)
                curr = el.iLeftIndex;
            else
                break;
        }
        return curr;
    }

    void* pLessFunc;
    UtlMemory<Node<Key, Value>> utlMemory;
    int iRootIndex;
    int iNumElements;
    int iFirstFree;
    int iLastAllock;
    Node<Key, Value>* pElements;
};

#endif