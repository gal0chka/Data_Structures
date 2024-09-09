#include "VectorStack.h"

void VectorStack::push(const ValueType& value)
{
    Vector.push_back(value);
}

void VectorStack::pop()
{
    if(!Vector.empty())
        Vector.pop_back();
}

const ValueType& VectorStack::top() const
{
    return Vector.back();
}

bool VectorStack::isEmpty() const
{
    return Vector.empty();
}

size_t VectorStack::size() const
{
    return Vector.size();
}