#pragma once

#include <stdexcept>
#include "Sequence.h"
#include "MutableSequence.h"
#include "DynamicArray.h"

template <typename T>
class SegmentedDeque {
private:
    int bufSize; // длина сегмента
    int size = 0; // кол-во элементов во всех сегментах
    DynamicArray<T*>* arrayPtr = nullptr; // массив указателей на сегменты.
    int offsetTail = 0; // отступ, чтобы создать новый элемент сзади
    int offsetHead = 0; // отступ, чтобы создать новый элемент спереди

    void increaseHead() {
        arrayPtr->append(new T[bufSize]);
    }

    void decreaseHead() {
        arrayPtr->resize(arrayPtr->getSize() - 1);
    }

    void increaseTail() {
        arrayPtr->prepend(new T[bufSize]);
    }

    void decreaseTail() { // увы, нет соответствующего метода в DynamicArray
        DynamicArray<T*>* bufArrayPtr = new DynamicArray<T*>(arrayPtr->getSize() - 1);
        for (int i = 0; i < arrayPtr->getSize() - 1; ++i) {
            bufArrayPtr->set(i, (*arrayPtr)[i + 1]);
        }
        delete arrayPtr;
        arrayPtr = bufArrayPtr;
    }

    // const - метод, так как используется в [] const
    T& getPrivate(int index) const {
        if (index < 0 || index >= size) throw std::out_of_range("Entered index is out of range.\n");
        int segmentIndex = index / bufSize;
        index %= bufSize;
        if (offsetTail == 0) return (*arrayPtr)[segmentIndex][index - offsetTail];
        if (offsetTail < index + 1) return (*arrayPtr)[segmentIndex + 1][index - offsetTail];
        return (*arrayPtr)[segmentIndex][(bufSize - offsetTail + index) % bufSize];
    }

    void deleteDeque() {
        if (size == 0) return;
        for (int i = 0; i < arrayPtr->getSize(); ++i) {
            delete[] (*arrayPtr)[i];
        }
        delete arrayPtr;
    }

public:
    SegmentedDeque(int bufSize = 4): bufSize(bufSize) {}

    SegmentedDeque(const Sequence<T>& other, int bufSize = 4): bufSize(bufSize) {
        for (int i = 0; i < other.getLength(); ++i) {
            append(other.get(i));
        }
    }

    SegmentedDeque(T* array, int size, int bufSize = 4): bufSize(bufSize) {
        for (int i = 0; i < size; ++i) {
            append(array[i]);
        }
    }

    SegmentedDeque(const DynamicArray<T>& other, int bufSize = 4): bufSize(bufSize) {
        for (int i = 0; i < other.getSize(); ++i) {
            append(other.get(i));
        }
    }

    SegmentedDeque(const SegmentedDeque<T>& other, int bufSize = 4): bufSize(bufSize) {
        for (int i = 0; i < other.getLength(); ++i) {
            append(other.get(i));
        }
    }

    ~SegmentedDeque() {
        deleteDeque();
    }

    T& operator[](int index) {
        return getPrivate(index);
    }

    const T& operator[](int index) const {
        return getPrivate(index);
    }

    SegmentedDeque<T>& operator=(const SegmentedDeque<T>& other) {
        // удаляем текущий дек
        deleteDeque();

        size = 0;
        offsetTail = 0;
        offsetHead = 0;
        bufSize = other.bufSize;

        // копируем элементы
        for (int i = 0; i < other.getLength(); ++i) {
            append(other.get(i));
        }
        return *this;
    }

    int getLength() const {
        return size;
    }

    const T& get(int index) const {
        // если index вне допустимых значений, то getPrivate(...) выбросит исключение
        return (*this)[index];
    }

    const T& getFirst() const {
        if (size == 0) throw std::out_of_range("You tried to get value from the empty SegmentedDeque.\n");
        return (*this)[0];
    }

    const T& getLast() const {
        if (size == 0) throw std::out_of_range("You tried to get value from the empty SegmentedDeque.\n");
        return (*this)[size - 1];
    }

    void set(int index, const T& item) {
        (*this)[index] = item;
    }

    void append(const T& item) {
        if (size == 0) arrayPtr = new DynamicArray<T*>(0);
        if (offsetHead == 0) { // добавляем сегмент в конец
            increaseHead();
            (*arrayPtr)[arrayPtr->getSize() - 1][offsetHead] = item;
            offsetHead = 1;
        } else { // не добавляем сегмент
            (*arrayPtr)[arrayPtr->getSize() - 1][offsetHead] = item;
            offsetHead = (offsetHead + 1) % bufSize;
        }
        ++size;
    }

    void prepend(const T& item) {
        if (size == 0) arrayPtr = new DynamicArray<T*>(0);
        if (offsetTail == 0) { // добавляем сегмент в начало
            increaseTail();
            (*arrayPtr)[0][bufSize - 1] = item;
            ++offsetTail;
        } else { // не добавляем сегмент
            (*arrayPtr)[0][bufSize - offsetTail - 1] = item;
            offsetTail = (offsetTail + 1) % bufSize;
        }
        ++size;
    }

    void insertAt(int index, const T& item) {
        if (index < 0 || index > size) throw std::out_of_range("Entered index is out of range.\n");
        if (size == 0) arrayPtr = new DynamicArray<T*>(0);
        if (index == size) {
            append(item);
            return;
        }
        prepend(item);
        for (int i = 1; i <= index; ++i) {
            (*this)[i - 1] = (*this)[i];
        }
        (*this)[index] = item;
    }

    void popHead() {
        if (size == 0) throw std::out_of_range("You tried to pop element from the empty deque.\n");
        if (offsetHead == 1) { // удаляем последний сегмент
            decreaseHead();
            offsetHead = 0;
        } else { // не удаляем последний сегмент
            if (offsetHead == 0) {
                offsetHead = bufSize;
            }
            --offsetHead;
        }
        --size;
    }

    void popTail() {
        if (size == 0) throw std::out_of_range("You tried to pop element from the empty deque.\n");
        if (offsetTail == 1) { // удаляем первый сегмент
            decreaseTail();
            offsetTail = 0;
        } else { // не удаляем сегмент
            if (offsetTail == 0) {
                offsetTail = bufSize;
            }
            --offsetTail;
        }
        --size;
    }

    void removeAt(int index) {
        if (index < 0 || index >= size) throw std::out_of_range("You tried to delete element from the empty deque.");
        for (int i = index + 1; i < size; ++i) {
            (*this)[i - 1] = (*this)[i];
        }
        popHead();
    }

    int getBufSize() const {
        return bufSize;
    }
};