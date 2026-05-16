#pragma once   

template <typename T>
class DynamicArray

{
private:
    T *data;
    size_t size; 
public:
    DynamicArray() : data{nullptr}, size{0} {}

    DynamicArray(size_t n) : size{n}
    {
        if (n == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[n];
        }
    }

    DynamicArray(const T *items, size_t n) : size{n}
    {
        if (n == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[n];
            std::copy(items, items + n, data);
        }
    }

    DynamicArray(const DynamicArray &other) : size{other.size}
    {
        if (size == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[size];
            std::copy(other.data, other.data + size, data);
        }
    }

    ~DynamicArray()
    {
        delete[] data;
    }
};