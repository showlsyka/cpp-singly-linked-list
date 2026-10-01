#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

template <typename Type> class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type &val, Node *next) : value(val), next_node(next) {}
        Type value;
        Node *next_node = nullptr;
    };

    template <typename ValueType> class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node *node) : node_(node) {
            // Реализуйте конструктор самостоятельно
        }

      public:
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType *;
        // Тип ссылки на итерируемое значение
        using reference = ValueType &;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type> &other) noexcept : node_(other.node_) {
            // Реализуйте конструктор самостоятельно
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;
    

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept {

            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен ==
        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept {
            return !(*this == rhs);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept {
            return node_ == rhs.node_;
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен ==
        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept {
            return !(*this == rhs);
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к
        // неопределённому поведению
        BasicIterator &operator++() noexcept {

            node_ = node_->next_node;

            return *this;
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            BasicIterator copy = *this;

            ++(*this);

            return copy;
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
            // Не реализовано
            // Заглушка. Реализуйте оператор самостоятельно
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
            // Заглушка. Реализуйте оператор самостоятельно
        }

      private:
        Node *node_ = nullptr;
    };

  public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {

            return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {

        Node* non_const_node = const_cast<Node*>(&head_);

        return ConstIterator(non_const_node);

    }

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept { return Iterator(head_.next_node); }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного
    // списка Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому
    // поведению
    [[nodiscard]] Iterator end() noexcept { return Iterator(nullptr); }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator(head_.next_node); }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом
    // односвязного списка Разыменовывать этот итератор нельзя — попытка разыменования приведёт к
    // неопределённому поведению Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept { return ConstIterator(nullptr); }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept { return ConstIterator(head_.next_node); }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом
    // односвязного списка Разыменовывать этот итератор нельзя — попытка разыменования приведёт к
    // неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept { return ConstIterator(nullptr); }

    SingleLinkedList(std::initializer_list<Type> values) : head_(), size_(0) {
        for (auto it = std::rbegin(values); it != std::rend(values); ++it) {
            PushFront(*it);
        }
    }

    SingleLinkedList(const SingleLinkedList &other) : head_(), size_(0) {

        Node *tail = &head_;

        for (Node *current = other.head_.next_node; current != nullptr;
             current = current->next_node) {
            tail->next_node = new Node(current->value, nullptr);
            tail = tail->next_node;
            ++size_;
        }
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs) {

        if (this != &rhs) {
            SingleLinkedList copy(rhs);
            swap(copy);
        }

        return *this;
    }

    void swap(SingleLinkedList &other) noexcept {

        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);

        // Реализуйте обмен содержимого списков самостоятельно
    }

  public:
    SingleLinkedList() { head_.next_node = nullptr; }

    ~SingleLinkedList() { Clear(); }

    void PushFront(const Type &value) {
        Node *old_node = head_.next_node;
        head_.next_node = new Node(value, old_node);

        ++size_;
    }

    void PopFront() noexcept {
        Node* old_node = head_.next_node;

        head_.next_node = old_node->next_node;

        delete old_node;
        --size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* new_node = new Node(value, pos.node_->next_node);

        pos.node_->next_node = new_node;

        ++size_;

        return Iterator(new_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {

        Node* node_to_delete = pos.node_->next_node;

        pos.node_->next_node = node_to_delete->next_node;

        delete node_to_delete;

        --size_;

        return Iterator(pos.node_->next_node);
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            Node *old_node = head_.next_node;
            head_.next_node = head_.next_node->next_node;

            delete old_node;
            --size_;
        }
    }

    // Возвращает количество элементов в списке
    [[nodiscard]] size_t GetSize() const noexcept { return size_; }

    // Сообщает, пустой ли список
    [[nodiscard]] bool IsEmpty() const noexcept { return head_.next_node == nullptr; }

  private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {

    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    // Заглушка. Реализуйте сравнение самостоятельно
    return !(lhs < rhs);
}
