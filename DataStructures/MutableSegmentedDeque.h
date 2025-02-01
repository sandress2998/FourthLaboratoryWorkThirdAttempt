#pragma once

#include <stdexcept>
#include "Sequence.h"
#include "MutableSequence.h"
#include "DynamicArray.h"
#include "SegmentedDeque.h"

template <typename T>
class MutableSegmentedDeque: public MutableSequence<T> {
private:
    SegmentedDeque<T>* deque;

public:
    MutableSegmentedDeque(int bufSize = 4): deque(new SegmentedDeque<T>(bufSize)) {}
    MutableSegmentedDeque(const Sequence<T>& other, int bufSize = 4): deque(new SegmentedDeque<T>(other, bufSize)) {}
    MutableSegmentedDeque(T* array, int size, int bufSize = 4): deque(new SegmentedDeque<T>(array, size, bufSize)) {}
    MutableSegmentedDeque(const DynamicArray<T>& other, int bufSize = 4): deque(new SegmentedDeque<T>(other, bufSize)) {}
    MutableSegmentedDeque(const MutableSegmentedDeque<T>& other, int bufSize = 4): deque(new SegmentedDeque<T>(*other.deque, bufSize)) {}

    ~MutableSegmentedDeque() override {
        delete deque;
    }

    T& operator[](int index) override {
        return (*deque)[index];
    }

    const T& operator[](int index) const override {
        return (*deque)[index];
    }

    MutableSegmentedDeque<T>& operator=(const MutableSegmentedDeque<T>& other) {
        (*deque) = (*other.deque);
        return *this;
    }

    int getLength() const override {
        return deque->getLength();
    }

    const T& get(int index) const override {
        return (*this)[index];
    }

    const T& getFirst() const override {
        //if (size == 0) throw std::out_of_range("You tried to get value from the empty MutableSegmentedDeque.\n");
        return deque->getFirst();
    }

    const T& getLast() const override {
        //if (size == 0) throw std::out_of_range("You tried to get value from the empty MutableSegmentedDeque.\n");
        return deque->getLast();
    }

    void set(int index, const T& item) override {
        deque->set(index, item);
    }

    void append(const T& item) override {
        deque->append(item);
    }

    void prepend(const T& item) override {
        deque->prepend(item);
    }

    void insertAt(int index, const T& item) override {
        deque->insertAt(index, item);
    }

    void popHead() {
        deque->popHead();
    }

    void popTail() {
        deque->popTail();
    }

    void removeAt(int index) {
        //if (index < 0 || index >= size) throw std::out_of_range("You tried to delete element from the empty deque.\n");
        deque->removeAt(index);
    }

    MutableSegmentedDeque<T>* concat(const Sequence<T>& other) const override {
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>(*this);
        for (int i = 0; i < other.getLength(); ++i) {
            result->append(other.get(i));
        }
        return result;
    }

    MutableSegmentedDeque<T>* getSubsequence(int startIndex, int endIndex) const override {
        if (startIndex > endIndex || startIndex < 0 || endIndex >= getLength()) throw std::out_of_range("Entered indices are out of range.\n");
        MutableSegmentedDeque<T>* result = new MutableSegmentedDeque<T>;
        for (int i = startIndex; i <= endIndex; ++i) {
            result->append(get(i));
        }
        return result;
    }

    int getBufSize() const {
        return deque->getBufSize();
    }
};
