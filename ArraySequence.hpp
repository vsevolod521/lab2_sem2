#pragma once
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <functional>
#include <stdexcept>

template <typename T>
class MutableArraySequence;

template <typename T>
class ImmutableArraySequence;

template <typename T>
class BaseArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T>* storage;

    BaseArraySequence() : storage(new DynamicArray<T>()) {}
    
    BaseArraySequence(const T* source, size_t count) : storage(new DynamicArray<T>(source, count)) {}
    
    BaseArraySequence(const BaseArraySequence& other) : storage(new DynamicArray<T>(*other.storage)) {}

    virtual ~BaseArraySequence()
    {
        delete storage;
    }

public:
    T GetFirst() const override
    {
        return storage->Get(0);
    }

    T GetLast() const override
    {
        return storage->Get(storage->GetSize() - 1);
    }

    T Get(size_t index) const override
    {
        return storage->Get(index);
    }

    size_t GetLength() const override
    {
        return storage->GetSize();
    }

    Sequence<T>* GetSubsequence(size_t start, size_t end) const override
    {
        size_t subLen = end - start + 1;
        T* subData = new T[subLen];
        for (size_t i = 0; i < subLen; ++i)
        {
            subData[i] = storage->Get(start + i);
        }

        Sequence<T>* result;
        if (dynamic_cast<const MutableArraySequence<T>*>(this))
        {
            result = new MutableArraySequence<T>(subData, subLen);
        }
        else
        {
            result = new ImmutableArraySequence<T>(subData, subLen);
        }

        delete[] subData;
        return result;
    }

};