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


    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            delete[] data;
            size = other.size;
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
        return *this;
    }

    T Get(size_t index) const
    {
        return data[index];
    }

    void Set(size_t index, const T &value)
    {
        data[index] = value;
    }

    size_t GetSize() const
    {
        return size;
    }

    void Resize(size_t newSize)
    {
        if (newSize == size)
            return;

        T *newData = nullptr;
        if (newSize > 0)
        {
            newData = new T[newSize];
        }

        size_t copy_count = (newSize < size) ? newSize : size;
        if (copy_count > 0 && data != nullptr)
        {
            std::copy(data, data + copy_count, newData);
        }

        delete[] data;
        data = newData;
        size = newSize;
    }

    T &operator[](size_t index)
    {
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        return data[index];
    }

    T *begin() { return data; }
    T *end() { return data + size; }

    const T *begin() const { return data; }
    const T *end() const { return data + size; }
};
