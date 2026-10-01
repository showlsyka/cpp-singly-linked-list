#include <algorithm>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

template <typename Type> class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type &val, Node *next) : value(val), next_node(next) {}
        Type value;
        Node *next_node = nullptr;
    };

    template <typename ValueType> class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node *node) : node_(node) {
        }

      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type> &other) noexcept : node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;
    

        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept {

            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator &operator++() noexcept {

            node_ = node_->next_node;

            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator copy = *this;

            ++(*this);

            return copy;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }

      private:
        Node *node_ = nullptr;
    };

  public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator before_begin() noexcept {

            return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {

        Node* non_const_node = const_cast<Node*>(&head_);

        return ConstIterator(non_const_node);

    }

    [[nodiscard]] Iterator begin() noexcept { return Iterator(head_.next_node); }

    [[nodiscard]] Iterator end() noexcept { return Iterator(nullptr); }

    [[nodiscard]] ConstIterator begin() const noexcept { return ConstIterator(head_.next_node); }

    [[nodiscard]] ConstIterator end() const noexcept { return ConstIterator(nullptr); }

    [[nodiscard]] ConstIterator cbegin() const noexcept { return ConstIterator(head_.next_node); }

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

    [[nodiscard]] size_t GetSize() const noexcept { return size_; }

    [[nodiscard]] bool IsEmpty() const noexcept { return head_.next_node == nullptr; }

  private:
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
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return !(lhs < rhs);
}
