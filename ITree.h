#pragma once

template<typename T>
class ITree {
public:
    virtual void insert(const T& value) = 0;
    virtual void remove(const T& value) = 0;
    virtual bool search(const T& value) const = 0;
    virtual void print() const = 0;
};