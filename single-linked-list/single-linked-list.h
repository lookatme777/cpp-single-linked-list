#include <cstddef>
#include <iterator>
#include <memory>
#include <sstream>
#include <type_traits>
#include <cassert>
#include <iostream>
#include <utility>
#include <algorithm>
#include <string_view>
#include <cassert>

using namespace std;


template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };


    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator(const BasicIterator<const Type>& other) noexcept {
            node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator<ValueType>& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return (node_ != rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ == rhs.node_);
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return (node_ != rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            if (node_) { node_ = node_->next_node; };
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            if (node_) { node_ = node_->next_node; };
            assert(node_ != nullptr);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() : size_(0) {}

    ~SingleLinkedList() {
        Clear();
    }
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    SingleLinkedList(std::initializer_list<Type> values) {

        SingleLinkedList tmp;
        for (auto item : values) {
            tmp.PushBack(item);
        }

        swap(tmp);
    }

    SingleLinkedList(const SingleLinkedList& other) {

        SingleLinkedList tmp;
        for (auto item : other) {
            tmp.PushBack(item);
        }
        swap(tmp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return head_.next_node == nullptr;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        if (!last_) {
            last_ = head_.next_node;
        }
        ++size_;
    }

    void PushBack(const Type& value) {
        if (last_) {
            last_->next_node = new Node(value, nullptr);
            last_ = last_->next_node;
        }
        else {
            head_.next_node = new Node(value, head_.next_node);
            last_ = head_.next_node;
        }
        ++size_;
    }

    void Clear() noexcept {
        while (head_.next_node) {
            Node* next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next;
        }
        head_.next_node = nullptr;
        last_ = nullptr;
        size_ = 0;
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node;
        ++size_;
        return Iterator{ pos.node_->next_node };
    }

    void PopFront() {
        auto temp = head_.next_node;
        head_.next_node = temp->next_node;
        delete temp;
        size_--;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        if (pos.node_->next_node != nullptr) {
            auto to_delete = pos.node_->next_node;
            pos.node_->next_node = to_delete->next_node;
            delete to_delete;
            --size_;
        }
        return Iterator{ pos.node_->next_node };
    }


private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
    Node* last_ = nullptr;
};


template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return ((lhs < rhs) || (lhs == rhs));
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return ((lhs > rhs) || (lhs == rhs));
}
