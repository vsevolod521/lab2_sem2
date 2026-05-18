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

    Sequence<T>* Append(const T& item) const override
    {
        return static_cast<BaseArraySequence<T>*>(GetInstance())->AppendImpl(item);
    }

    Sequence<T>* Prepend(const T& item) const override
    {
        return static_cast<BaseArraySequence<T>*>(GetInstance())->PrependImpl(item);
    }

    Sequence<T>* InsertAt(const T& item, size_t index) const override
    {
        return static_cast<BaseArraySequence<T>*>(GetInstance())->InsertAtImpl(item, index);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override
    {
        return static_cast<BaseArraySequence<T>*>(GetInstance())->ConcatImpl(other);
    }

    Sequence<T>* Where(std::function<bool(const T&)> predicate) const override
    {
        DynamicArray<T>* filtered = new DynamicArray<T>();
        for (size_t i = 0; i < GetLength(); ++i)
        {
            T val = Get(i);
            if (predicate(val))
            {
                filtered->Resize(filtered->GetSize() + 1);
                filtered->Set(filtered->GetSize() - 1, val);
            }
        }

        Sequence<T>* result;
        if (dynamic_cast<const MutableArraySequence<T>*>(this))
        {
            result = new MutableArraySequence<T>(*filtered);
        }
        else
        {
            result = new ImmutableArraySequence<T>(*filtered);
        }

        delete filtered;
        return result;
    }

protected:
    BaseArraySequence<T>* AppendImpl(const T& item)
    {
        storage->Resize(storage->GetSize() + 1);
        storage->Set(storage->GetSize() - 1, item);
        return this;
    }

    BaseArraySequence<T>* PrependImpl(const T& item)
    {
        storage->Resize(storage->GetSize() + 1);
        for (size_t i = storage->GetSize() - 1; i > 0; --i)
        {
            storage->Set(i, storage->Get(i - 1));
        }
        storage->Set(0, item);
        return this;
    }

    BaseArraySequence<T>* InsertAtImpl(const T& item, size_t index)
    {
        storage->Resize(storage->GetSize() + 1);
        for (size_t i = storage->GetSize() - 1; i > index; --i)
        {
            storage->Set(i, storage->Get(i - 1));
        }
        storage->Set(index, item);
        return this;
    }

    BaseArraySequence<T>* ConcatImpl(const Sequence<T>* other)
    {
        if (!other) return this;
        
        size_t otherLen = other->GetLength();
        size_t currentLen = storage->GetSize();
        storage->Resize(currentLen + otherLen);
        
        for (size_t i = 0; i < otherLen; ++i)
        {
            storage->Set(currentLen + i, other->Get(i));
        }
        return this;
    }

    virtual Sequence<T>* GetInstance() const = 0;
    virtual Sequence<T>* CloneSequence() const = 0;
};

template <typename T>
class MutableArraySequence : public BaseArraySequence<T>
{
public:
    MutableArraySequence() : BaseArraySequence<T>() {}
    
    MutableArraySequence(const T* items, size_t count) : BaseArraySequence<T>(items, count) {}
    
    MutableArraySequence(const MutableArraySequence& other) : BaseArraySequence<T>(other) {}

protected:
    Sequence<T>* GetInstance() const override
    {
        return const_cast<MutableArraySequence<T>*>(this);
    }

    Sequence<T>* CloneSequence() const override
    {
        return new MutableArraySequence<T>(*this);
    }
};

template <typename T>
class ImmutableArraySequence : public BaseArraySequence<T>
{
public:
    ImmutableArraySequence() : BaseArraySequence<T>() {}
    
    ImmutableArraySequence(const T* items, size_t count) : BaseArraySequence<T>(items, count) {}
    
    ImmutableArraySequence(const ImmutableArraySequence& other) : BaseArraySequence<T>(other) {}

protected:
    Sequence<T>* GetInstance() const override
    {
        return this->CloneSequence();
    }

    Sequence<T>* CloneSequence() const override
    {
        return new ImmutableArraySequence<T>(*this);
    }
};
