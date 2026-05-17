
template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node *next;
        explicit Node(const T &val) : value{val}, next{nullptr} {}
    };

    Node *head;
    Node *tail;

    void Clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

public:
    class Iterator
    {
    private:
        Node *ptr;

    public:
        Iterator(Node *p) : ptr{p} {}

        T &operator*() const { return ptr->value; }

        Iterator &operator++()
        {
            if (ptr)
                ptr = ptr->next;
            return *this;
        }

        bool operator==(const Iterator &other) const
        {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator &other) const
        {
            return ptr != other.ptr;
        }

        
    };

    LinkedList() : head{nullptr}, tail{nullptr} {}
    LinkedList(const T *items, size_t count) : head{nullptr}, tail{nullptr}
    {
        for (size_t i = 0; i < count; ++i)
        {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList &other) : head{nullptr}, tail{nullptr}
    {
        Node *current = other.head;
        while (current != nullptr)
        {
            Append(current->value);
            current = current->next;
        }
    }

    ~LinkedList()
    {
        Clear();
    }
};