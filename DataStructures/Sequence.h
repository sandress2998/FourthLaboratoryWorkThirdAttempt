#pragma once
#include <iostream>

template <class T>
class Sequence {
public:
    virtual ~Sequence() = default;
    virtual const T& getFirst() const = 0;
    virtual const T& getLast() const = 0;
    virtual const T& get(int index) const = 0; // int

    virtual int getLength() const = 0;

    virtual Sequence<T>* getSubsequence(int startIndex, int endIndex) const = 0;
    virtual Sequence<T>* concat(const Sequence<T>& secondSequence) const = 0;

    virtual const T& operator [] (int index) const = 0;

    friend std::ostream& operator << ( std::ostream& os, const Sequence<T>& value ) {
        if (value.getLength() == 0) return os;
        int i = 0;
        for (; i < value.getLength() - 1; ++i) {
            os << value.get(i) << " ";
        }
        os << value.get(i) << "\n";
        return os;
    }
};
