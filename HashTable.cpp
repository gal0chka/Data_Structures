#include "HashTable.h"
#include <functional>
#include <iostream>

// Конструктор
HashTable::HashTable(size_t size) noexcept 
    : _capacity(static_cast<int32_t>(size)), _filled(0), table(size) {}

// Деструктор
HashTable::~HashTable() {}

// Хеш-функция
// size_t HashTable::hash_function(const KeyType &key) const {
//     std::hash<KeyType> hashObj;
//     return hashObj(key) % _capacity;
// }

size_t HashTable::hash_function(const std::string &key) const {
    unsigned int hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    return hash % _capacity;
}

// Вставка элемента в хеш-таблицу
void HashTable::insert(const KeyType &key, const ValueType &value) {
    size_t index = hash_function(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    table[index].emplace_back(key, value);
    _filled++;

    if (getLoadFactor() > 0.75) {
        rehash();
    }
}

// Поиск элемента в хеш-таблице
bool HashTable::find(const KeyType &key, ValueType &value) const {
    size_t index = hash_function(key);
    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            value = pair.second;
            return true;
        }
    }
    return false;
}

// Удаление элемента из хеш-таблицы
void HashTable::remove(const KeyType &key) {
    size_t index = hash_function(key);
    auto& chain = table[index];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->first == key) {
            chain.erase(it);
            _filled--;
            return;
        }
    }
}

// Получение элемента по ключу
ValueType& HashTable::operator[](const KeyType &key) {
    size_t index = hash_function(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    table[index].emplace_back(key, ValueType());
    _filled++;
    return table[index].back().second;
}

// Загрузка хеш-таблицы
double HashTable::getLoadFactor() {
    return static_cast<double>(_filled) / _capacity;
}

// Реорганизация хеш-таблицы
void HashTable::rehash() {
    _capacity *= 2;
    std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(_capacity);

    for (const auto& chain : table) {
        for (const auto& pair : chain) {
            size_t new_index = hash_function(pair.first);
            new_table[new_index].emplace_back(pair);
        }
    }

    table.swap(new_table);
}
