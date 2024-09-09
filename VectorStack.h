#pragma once
#include "StackImplementation.h"
#include <vector>

class VectorStack : public IStackImplementation 
{
public:
    VectorStack() = default;

    VectorStack(const ValueType* valueArray, const size_t arraySize);

    virtual ~VectorStack() = default;

    void push(const ValueType& value) override;

    void pop() override;

    const ValueType& top() const override;

    bool isEmpty() const override;

    size_t size() const override;
private:
    std::vector<ValueType> Vector;
};