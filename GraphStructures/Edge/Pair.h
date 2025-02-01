#pragma once

template <typename T>
class Pair {
    T first;
    T second;
public:
    Pair(const T& first, const T& second): first(first), second(second) {}
    
    T& getFirst() {
        return first;
    }

    T& getSecond() {
        return second;
    }
};