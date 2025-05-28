#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <vector>
#include "Peserta.h" // Assuming Peserta.h is in the same directory or include path

// Forward declaration of Peserta struct if Peserta.h is not included,
// but it's better to include Peserta.h as it contains the full definition.
// struct Peserta; 

class queuePeserta
{
private:
    std::vector<Peserta> data;

public:
    bool empty() const
    {
        return data.empty();
    }

    size_t size() const
    {
        return data.size();
    }

    void push(const Peserta &p)
    {
        data.push_back(p);
    }

    void pop()
    {
        if (!data.empty())
        {
            data.erase(data.begin());
        }
    }

    Peserta& front()
    {
        // Note: Calling front() on an empty queue is undefined behavior.
        // Adding a check or ensuring it's not called on an empty queue is good practice.
        return data.front();
    }

    // Optional: const version of front()
    const Peserta& front() const
    {
        return data.front();
    }
};

class stackPeserta
{
private:
    std::vector<Peserta> data;

public:
    bool empty() const
    {
        return data.empty();
    }

    size_t size() const
    {
        return data.size();
    }

    void push(const Peserta &p)
    {
        data.push_back(p);
    }

    void pop()
    {
        if (!data.empty())
        {
            data.pop_back();
        }
    }

    Peserta &top()
    {
        // Note: Calling top() on an empty stack is undefined behavior.
        // Adding a check or ensuring it's not called on an empty stack is good practice.
        return data.back();
    }

    // Optional: const version of top()
    const Peserta& top() const
    {
        return data.back();
    }
};

#endif // DATASTRUCTURES_H
