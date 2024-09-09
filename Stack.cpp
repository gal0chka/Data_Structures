#include <iostream>
#include "Stack.h"
#include "StackImplementation.h"
#include "VectorStack.h"
#include "ListStack.h"

Stack::Stack(StackContainer container) : _containerType(container) 
{
    switch (container) 
    {
        case StackContainer::Vector:
            _pimpl = new VectorStack();
            break;
        case StackContainer::List:
            _pimpl = new ListStack();
            break;
        default:
            _pimpl = new VectorStack();
            break;
    }
}

Stack::Stack(const ValueType* valueArray, const size_t arraySize, StackContainer container) : Stack(container) 
{
    for (size_t i = 0; i < arraySize; ++i) 
    {
        push(valueArray[i]);
    }
}

Stack::Stack(const Stack& copyStack) : _containerType(copyStack._containerType) 
{
    *this = copyStack;
}

Stack& Stack::operator=(const Stack& copyStack) 
{
    if (this != &copyStack) 
    {
        delete _pimpl;
        _pimpl = nullptr;
        _containerType = copyStack._containerType;

        switch (_containerType) {
            case StackContainer::Vector:
                _pimpl = new VectorStack(*dynamic_cast<VectorStack*>(copyStack._pimpl));
                break;
            case StackContainer::List:
                _pimpl = new ListStack(*dynamic_cast<ListStack*>(copyStack._pimpl));
                break;
            default:
                _pimpl = nullptr;
                break;
        }
    }
    return *this;
}

Stack::Stack(Stack&& moveStack) noexcept : _pimpl(moveStack._pimpl), _containerType(moveStack._containerType) 
{
    *this = std::move(moveStack);
}

Stack& Stack::operator=(Stack&& moveStack) noexcept 
{
    if (this != &moveStack) 
    {
        delete _pimpl;
        _pimpl = moveStack._pimpl;
        _containerType = moveStack._containerType;
        moveStack._pimpl = nullptr;
    }
    return *this;
}

Stack::~Stack() 
{
    delete _pimpl;
}

void Stack::push(const ValueType& value) 
{
    _pimpl->push(value);
}

void Stack::pop() 
{
    _pimpl->pop();
}

const ValueType& Stack::top() const 
{
    return _pimpl->top();
}

bool Stack::isEmpty() const 
{
    return _pimpl->isEmpty();
}

size_t Stack::size() const 
{
    return _pimpl->size();
}