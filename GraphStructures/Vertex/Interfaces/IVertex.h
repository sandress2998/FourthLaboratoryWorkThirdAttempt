#pragma once
#include "../DataStructures/MutableSegmentedDeque.h"

template <typename T>
class IVertex {
public:
    IVertex() = default;
    
    virtual ~IVertex() {}
};