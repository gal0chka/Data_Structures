#include "new_BST.h"
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

//! Конструктор с параметрами
BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right, bool flag)
        : keyValuePair(make_pair(key, value)), parent(parent), left(left), right(right), flag(flag) {}

//! конструктор копирования(глубокое)
BinarySearchTree::Node::Node(const Node &other) {
    keyValuePair.first = other.keyValuePair.first;
    keyValuePair.second = other.keyValuePair.second;
    flag = other.flag; // Наша node
    if (other.left == nullptr) {
        left = nullptr;
    } else {
        left = new Node(*other.left); // КАК ТЫ РАБОТАЕШЬ  -ЧУДО
        left->parent = this;
    }
    if (other.right == nullptr) {
        right = nullptr;
    } else {
        right = new Node(*other.right);
        parent = other.parent;
        right->parent = this;

    }
}

//! Оператор сравнения ==
bool BinarySearchTree::Node::operator==(const Node &other) const {
    return ((keyValuePair.first == other.keyValuePair.first) && (keyValuePair.second == other.keyValuePair.second)
            && (parent == other.parent) && (left == other.left) && (right == other.right));
}


//! Вывод в консоль поддерева, где текущий узел - корень
void BinarySearchTree::Node::output_node_tree() const {
    if (flag) { //Наша нода не null
        cout << "Key: " << keyValuePair.first << ", Value: " << keyValuePair.second << endl;
    } else {
        cout << "(nill node)" << endl;
    }
    if (left) {
        left->output_node_tree();
    }
    if (right) {
        right->output_node_tree();
    }
}

//! Вставить новый узел в поддерево, где текущий узел - корень
void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
    if (key < keyValuePair.first) {
        //Создаем наследника
        if (left == nullptr) {
            //This->parent нельзя(тупая ошибка)
            Node *node = new Node(key, value, this, nullptr, nullptr);
            left = node;
        } else {
            //Если уже есть, то делаем рекурсивную вставку
            left->insert(key, value);
        }
        //todo: распределяем в право
    } else if (key >= keyValuePair.first) {
        if (right == nullptr) {
            // Если никого нету, то создать узел(он правый для текущего)
            Node *node = new Node(key, value, this, nullptr, nullptr);
            right = node;
        } else {
            right->insert(key, value);
        }
    }
}

//######################################################
//######################################################
//######################################################
//######################################################
//######################################################
//! Удалить узел из поддерева, где текущий узел - корень
BinarySearchTree::Node *BinarySearchTree::Node::erase_node(Node *node, const Key &key) {
    if (node == nullptr) {
        return node;
    }

    if (key < node->keyValuePair.first) { //если ключ меньше
        node->left = erase_node(node->left, key); //рекурсивно идем влево
        return node;
    } else if (key > node->keyValuePair.first) { //если ключ больше
        node->right = erase_node(node->right, key); //рекурсивно идем вправо
        return node;
    }

    //если у ноды один ребенок или без детей
    if (node->left == nullptr) {
        Node *temp = node->right;
        delete node;
        return temp;
    } else if (node->right == nullptr) {
        Node *temp = node->left;
        delete node;
        return temp;
    }

    //если два ребенка. Ищем наименьший элемент в правом поддереве то есть самый
    //левый
    Node *temp = node->right; //правый элемент удаляемого
    Node *parent = node;      //родитель этого правого

    while (temp->left != nullptr) {
        parent = temp;
        temp = temp->left; //идем в  левый узел
    }
    node->keyValuePair = temp->keyValuePair;

    if (parent->left == temp) {
        parent->left = temp->right;
    } else {
        parent->right = temp->right;
    }

    delete temp;
    return node;
}


//! Конструктор копирования
BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) {
    _size = other._size;
    if (other._root != nullptr) {
        _root = new Node(*other._root);
    }
}

//! Оператор присваивания копированием
BinarySearchTree &BinarySearchTree::operator=(const BinarySearchTree &other) {
    if (this != &other) {
        this->~BinarySearchTree();
        _size = other._size; //todo по-другому
        if (other._root != nullptr) { //Разменование опасно
            _root = new Node(*other._root);
        } else {
            _root == nullptr;
        }
    }
    return *this;
}

//! Конструктор перемещения
BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
}

//! Оператор присваивания перемещением
BinarySearchTree &BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
    if (this != &other) {
        this->~BinarySearchTree(); //удалить всю прошлую инфу
        if (other._root != nullptr) { //Разменование опасно
            _root = other._root;
            other._root = nullptr;
        } else {
            _root = nullptr;
        }
        _size = other._size;
        other._size = 0;
    }
    return *this;
}
void BinarySearchTree::Node::Delete_Nodes(Node* top){
    if (top!= nullptr){
        Delete_Nodes(top->left);
        delete top->left;
        Delete_Nodes(top->right);
        delete top->right;
    }
}

//! Деструктор
BinarySearchTree::~BinarySearchTree() {
    //Todo: Будто надо почитать про рекурсивное удаление дерева
    if (_root != nullptr) {
        _root->Delete_Nodes(_root);
       // BinarySearchTree::Node::contInOrder
        delete _root;
    }
}

//   //! \brief Итератор бинарного дерева поиска
//   //! \note Обходит дерево последовательно от узла с меньшим ключом к узлу с
//   //! большим

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value> &BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() {
    return &(this->_node->keyValuePair);
}

const std::pair<Key, Value> *BinarySearchTree::Iterator::operator->() const {
    return &(this->_node->keyValuePair);
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (_node->right != nullptr) { //Существует правый
        _node = _node->right;
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }//todo: подумать
    else {// Если правого нет
        while ((_node->parent != nullptr) && (_node == _node->parent->right)) {
            _node = _node->parent;
            //движемся по правой ветке вверх, пока этот узел является правым дочерним
        }
        _node = _node->parent;
        // _node->parent = _node->parent->parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator tmp(this->_node);
    ++(*this);
    return tmp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node->left != nullptr) { //если у узла есть левый
        _node = _node->left; //то переходим к нему
        while (_node->right != nullptr) { //и идем от него по правой ветке до конца
            _node = _node->right;
        }
    } else {//если у узла нет левого
        Node *parent = _node->parent; //идем к родителю
        while ((parent != nullptr) && (_node == parent->left)) { //и движемся по левой ветке вверх, до тех пор,
            //пока этот узел является левым дочерним
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator tmp(this->_node);
    --(*this);
    return tmp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
    return (_node == other._node);
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
    return (_node != other._node);
}

//   //! Константный итератор бинарного дерева поиска
BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}


const std::pair<Key, Value> &BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value> *BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

//##############################################
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node->right != nullptr && !_node->right->flag) { //если у узла есть правый

        _node = _node->right; //то переходим к нему
        while (_node->left != nullptr) { //и идем от него по левой ветке до конца

            _node = _node->left;
        }
    } else { //если у узла нет правого

        Node *parent = _node->parent; //идем к родителю
        while (parent != nullptr && _node == parent->right) { //и движемся по правой ветке вверх
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator tmp = *this;
    ++(*this);
    return tmp;
}

//##################
BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node->left != nullptr) { //если у узла есть левый

        _node = _node->left; //то переходим к нему
        while (_node->right != nullptr) { //и идем от него по правой ветке до конца
            _node = _node->right;
        }
    } else { //если у узла нет левого
        Node *parent = _node->parent; //идем к родителю
        while (parent != nullptr && _node == parent->left) { //и движемся по левой ветке вверх, до тех пор,
            //пока этот узел является левым дочерним
            _node = parent;
            parent = parent->parent;
        }
        _node = parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator tmp = *this;
    --(*this);
    return tmp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
    return _node != other._node;
}
//! Вставить элемент с ключем key и значением value

void BinarySearchTree::insert(const Key &key, const Value &value) {
    if (_root) {
        DeleteNull();
        _root->insert(key, value);
    } else {
        _root = new Node(key, value);
    }
    InsertNull();
    _size++;
}

//! Удалить все элементы с ключем key
void BinarySearchTree::erase(const Key &key) {
    if (_root) {
        DeleteNull();
        _root = _root->erase_node(_root, key);
        _size--;
        InsertNull();
    }
    // Пока в дереве есть ключи key, удаляем их
    Iterator f = find(key);
    if (f != end()) {
        erase(key);
    }
}

//! Найти первый элемент в дереве, равный ключу key
BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
    Node *current = _root; //указатель на текущий узел

    while (current != nullptr) { //пока не дойдем до конца дерева

        if (key > current->keyValuePair.first && !current->right->flag) {
            //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (key < current->keyValuePair.first) {
            //если key меньше, чем у текущего узла, перейдем влево
            current = current->left;
        } else if (key == current->keyValuePair.first) { //если ключи равны
            return ConstIterator(current);
        } else {
            return cend();
        } //если ключа не найдено, вернуть cend()
    }
    return ConstIterator(nullptr);
}

//! Найти первый элемент в дереве, равный ключу key
BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
    Node *current = _root; //указатель на текущий узел

    while (current != nullptr) { //пока не дойдем до конца дерева

        if (current->right && key > current->keyValuePair.first && !current->right->flag) {
            //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (current->left && key < current->keyValuePair.first) {
            //если key меньше, чем у
            //текущего узла, перейдем влево
            current = current->left;
        } else if (key == current->keyValuePair.first) { //если ключи равны
            return Iterator(current);
        } else
            return end(); //если ключа не найдено, вернуть end()
    }
    return end();
}

//   /*!***********************************************************
//   Найти все элементы, у которых ключ равен key:
//     - первый итератор пары - первый элемент в дереве, равный key
//     - второй итератор пары - первый элемент в дереве больший, чем key

//   [pair.first, pair.second) - полуинтервал, содержащий все
//   элементы с ключем key
std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
    Iterator current = begin();
    int flag = 1;
    Iterator resBegin(nullptr);
    Iterator resEnd(nullptr);

    while (current != end()) {
        if ((*current).first == key) {
            if (flag) {
                resBegin = current;
                flag = 0;
            }
            Iterator temp = current;
            ++temp;
            if ((*temp).first > key || temp == end()) {
                resEnd = temp;
                break;
            }
        }
        current++;
    }
    return std::pair<Iterator, Iterator>(resBegin, resEnd);
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const {
    ConstIterator current = cbegin();
    int flag = 1;
    ConstIterator resBegin(nullptr);
    ConstIterator resEnd(nullptr);

    while (current != cend()) {
        if ((*current).first == key) {
            if (flag) {
                resBegin = current;
                flag = 0;
            }
            ConstIterator temp = current;
            ++temp;
            if ((*temp).first > key || temp == cend()) {
                resEnd = temp;
                break;
            }
        }
        current++;
    }
    return std::pair<ConstIterator, ConstIterator>(resBegin, resEnd);
}


//   //! Получить итератор на элемент с наименьшим ключем в дереве
BinarySearchTree::ConstIterator BinarySearchTree::min() const {
    Node *current = _root;
    while (current && current->left) {
        current = current->left;
    }
    return ConstIterator(current);
}

//   //! Получить итератор на элемент с ключем key с наибольшим значением
BinarySearchTree::ConstIterator BinarySearchTree::max() const {
    Node *current = _root; //нужно идти от корня
    while (current && current->right && !current->right->flag) {
        current = current->right; //в самый правый лист (не учитывая nil)
    }
    return ConstIterator(current);
}


//   //! Получить итератор на элемент с ключем key с наименьшим значением
BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
    Node *current = _root; //указатель на текущий узел
    Node *res = nullptr;
    Value min;
    int f = 1; //флаг для первичной инициализации min

    while (current != nullptr) { //пока не дойдем до конца дерева
        if (key > current->keyValuePair.first && !current->right->flag) {
            //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (key < current->keyValuePair.first) {
            //если key меньше, чем у текущего узла, перейдем влево
            current = current->left;
        } else { //если ключи равны
            if (current->keyValuePair.second < min || f) {
                //если значение текущего меньше min или min не проиницилизировано
                min = current->keyValuePair.second; //обновляем min
                res = current; //обновляем возвращаемый узел
                f = 0;         //обнуляем флаг
            }
            current = current->right; //переходим в правую ветку, т.к. узел с
            //одинаковым ключом уходит вправо
        }
    }
    return ConstIterator(res);
}

//   //! Получить итератор на элемент с ключем key с наибольшим значением
BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
    Node *current = _root; //указатель на текущий узел
    Node *res = nullptr;
    Value max;
    int f = 1; //флаг для первичной инициализации max

    while (current != nullptr) { //пока не дойдем до конца дерева

        if (key > current->keyValuePair.first &&
            !current->right->flag) { //если key больше, чем у текущего узла, перейдем вправо
            current = current->right;
        } else if (key < current->keyValuePair.first) { //если key меньше, чем у текущего
            //узла, перейдем влево
            current = current->left;
        } else { //если ключи равны
            if (current->keyValuePair.second > max || f) {
                //если значение текущего меньше max или max не проиницилизировано
                max = current->keyValuePair.second; //обновляем max
                res = current; //обновляем возвращаемый узел
                f = 0;         //обнуляем флаг
            }
            current = current->right; //переходим в правую ветку, т.к. узел с
            //одинаковым ключом уходит вправо
        }
    }
    return ConstIterator(res);
}


//   //! Получить итератор на первый элемент дерева (элемент с наименьшим key)
BinarySearchTree::Iterator BinarySearchTree::begin() {
    Node *current = _root;
    while (current && current->left) {
        current = current->left;
    }
    return Iterator(current);
}

//   //! Получить итератор на элемент, следующий за последним элементом дерева
//   //! \note Т.е. tree.end()-- == tree.max()
BinarySearchTree::Iterator BinarySearchTree::end() {
    Node *current = _root;
    while (current && current->right) {
        current = current->right;
    }
    return Iterator(current);
}

//   //! Получить константный итератор на начало
BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    return min();
}

//   //! Получить константный итератор на конец
BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    Node *current = _root;
    while (current && current->right) {
        current = current->right;
    }
    return ConstIterator(current);
}


//   //! Получить размер дерева
size_t BinarySearchTree::size() const { return _size; }

//   //! Вывести дерево в консоль
void BinarySearchTree::output_tree() {
    if (_root != nullptr) {
        _root->output_node_tree();
    } else {
        std::cout << "(Empty tree)" << std::endl;
    }
}


//   //! Удалить фиктивную ноду
void BinarySearchTree::DeleteNull() {
    Node *current = _root;
    if (!current) { return; }

    while (current->right) {
        current = current->right;
    }
    if (current != nullptr && current->flag) {
        current->parent->right = nullptr;
        delete current;
    }
}

//   //! Вставить фиктивную ноду
void BinarySearchTree::InsertNull() {
    if (_root) {
        Node *temp = new Node(0, 0);
        temp->flag = true;
        Node *current = _root;

        while (current->right) {
            current = current->right;
        }
        if (!current->flag) {
            current->right = temp;
            temp->parent = current;
        }
    }
}
