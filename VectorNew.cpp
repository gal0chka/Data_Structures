#include <iostream>
#include <stdlib.h>

#include "VectorNew.h"


Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
{
    
    _size = size;
    _multiplicativeCoef = coef;
    _capacity =  (size);
    _data = new ValueType[_capacity];
    for(int i = 0; i < size; ++i)
    {
        _data[i] = rawArray[i];
    }
}

Vector::Vector(const Vector& other) : 
_size(other._size),
 _multiplicativeCoef(other._multiplicativeCoef),
 _capacity(other._size)
{

    _data = new ValueType[_capacity];
    for(int i = 0; i < _size; ++i)
    {
        _data[i] = other._data[i];
    }   
}


Vector& Vector:: operator=(const Vector& other)
{
    if(this != &other)
    {
        delete[] _data;
        _size = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        _capacity = other._capacity;
        _data = new ValueType[_capacity];
        for(int i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }
    }
    return *this;
}


Vector::Vector(Vector&& other) noexcept:
_size(0),_multiplicativeCoef(2.0f),
_capacity(0),_data(nullptr)
{
    _size = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _capacity = other._capacity;
    _data = other._data;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 2.0f;
}

Vector& Vector:: operator=(Vector&& other) noexcept
{
    if (this != &other)
    {
        delete[] _data;
        _size = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        _capacity = other._capacity;
        _data = other._data;
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
        other._multiplicativeCoef = 2.0f;
    }
    return *this;
}

Vector::~Vector()
{
    delete[] _data;
}


void Vector::pushBack(const ValueType &value) {
  if (_size >= _capacity) {
    if (_capacity == 0) {
      _capacity = 1;
    }

    if (_data == nullptr) {
      _data = new ValueType[1];
    }
    reserve(_capacity * _multiplicativeCoef);
  }
  _data[_size++] = value;
}


void Vector::pushFront(const ValueType &value) {
  if (_capacity == 0) {
    _capacity = (int)_multiplicativeCoef;
    _data = new ValueType[(int)_multiplicativeCoef];
    _size = 1;
    _data[0] = value;
    return;
  }
  if (_size == _capacity) {
    reserve(_capacity * _multiplicativeCoef);
  }
  for (int i = _size; i > 0; i--) {
    _data[i] = _data[i - 1];
  }
  _data[0] = value;
  _size++;
}


void Vector::insert(const ValueType &value, size_t pos) {
  insert(&value, 1, pos);
}

void Vector::insert(const ValueType *values, size_t size, size_t pos) {
  if (_capacity == 0) {
    _capacity = 1;
    if (_data == nullptr) {
      _data = new ValueType[1];
    }
    _size = 1;
  }

  if (_size + size > _capacity) {
    reserve(_size + size);
  }

  for (size_t i = _size; i > pos; --i) {
    _data[i + size - 1] = _data[i - 1];
  }

  for (size_t i = 0; i < size; ++i) {
    _data[pos + i] = values[i];
  }
  _size += size;
}

void Vector::insert(const Vector &vector, size_t pos) {
  insert(vector._data, vector._size, pos);
}

void Vector::popBack() {
  if (_size > 0) {
    _size--;
  } else {
    throw std::out_of_range("Vector is empty");
  }
}

void Vector::popFront() {
  if (_size == 0 || _capacity == 0) {
    throw std::out_of_range("Vector is empty");
  } 

  for (int i = 1; i < _size; ++i) {
    _data[i - 1] = _data[i];
  }
  _size--;
}


void Vector::erase(size_t pos, size_t count)
{
    if((pos + count) >= _size)
    {
        std::copy(_data + _size, _data + _size,_data + pos);
        _size = pos;
    }
    else
    {
        std::copy(_data + pos + count, _data + _size,_data + pos);
        _size -= count;
    }
    
}

void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    if (endPos >= _size)
    {
        std::copy(_data + _size, _data + _size,_data + beginPos);
    }
    else
    {
        std::copy(_data + endPos, _data + _size,_data + beginPos);
    }
}

size_t Vector::size() const
{
    return _size;
}
size_t Vector::capacity() const
{
    return _capacity;
}

double Vector::loadFactor() const
{
    return _size/_capacity;
}

ValueType& Vector::operator[](size_t idx)
{
    return _data[idx];
}

const ValueType& Vector::operator[](size_t idx) const
{
    return _data[idx];
}




long long Vector::find(const ValueType& value) const
{
    long long par = -1;

    for(long long i = 0; i < _size; ++i)
    {
        if(_data[i] == value)
        {
            par = value;
            break;
        }
    }
    return par;
}

 void Vector::reserve(size_t capacity)
 {
    if (capacity > _capacity) {
        ValueType* mass = new ValueType[capacity];
        for (size_t i = 0; i < _size; ++i) {
            mass[i] =_data[i];
        }
        _capacity = capacity;
        delete[] _data;
        _data = mass;
    }
 }

 void Vector::shrinkToFit()
 {
    if (_size < _capacity) 
    {
        ValueType* mass = new ValueType[_size];
        for (size_t i = 0; i < _size; ++i) 
        {
            mass[i] = _data[i];
        }
        _capacity = _size;
        delete[] _data;
        _data = mass;
    }
}

Vector::Iterator::Iterator(ValueType* ptr):
_ptr(ptr)
{
}

ValueType& Vector::Iterator::operator*()
{
    return *_ptr;
}

const ValueType& Vector::Iterator::operator*() const
{
    return *_ptr;
}

ValueType* Vector::Iterator::operator->()
{
    return _ptr;
}

const ValueType* Vector::Iterator::operator->() const
{
    return _ptr;
}

Vector::Iterator Vector::Iterator:: operator++()
{
    _ptr ++;
    return *this;
}

Vector::Iterator Vector::Iterator:: operator++(int)
{
    Iterator temp = *this;
    ++_ptr;
    return temp;
}

bool Vector::Iterator:: operator==(const Iterator& other) const
{
    return _ptr == other._ptr;
}

bool Vector::Iterator:: operator!=(const Iterator& other) const
{
    return _ptr != other._ptr;
}

Vector::Iterator Vector::begin()
{
    return Iterator(_data);
}

Vector::Iterator Vector::end()
{
    return Iterator(_data + _size);
}