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
			if (node_) { node_ = node_->next_node; };
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			auto old_value(*this);
			if (node_) { node_ = node_->next_node; };
			return old_value;
		}

		[[nodiscard]] reference operator*() const noexcept {
			return node_->value;
		}

		[[nodiscard]] pointer operator->() const noexcept {
			return &node_->value;
		}

	private:
		Node* node_ = nullptr;
	};

public:
	//Node* head_.next_node = nullptr;
	//Node* first = nullptr;
	Node* last = nullptr;
	SingleLinkedList() : last(nullptr) {}

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
			SingleLinkedList tmp;
			for (auto item : rhs) {
				tmp.PushBack(item);
			}
			swap(tmp);
		}
		return *this;
	}

	void swap(SingleLinkedList& other) noexcept {
		Node* tmp = other.head_.next_node;
		other.head_.next_node = head_.next_node;
		head_.next_node = tmp;
		tmp = other.last;
		other.last = last;
		last = tmp;
	}

	[[nodiscard]] size_t GetSize() const noexcept {
		int size = 0;
		Node* p = head_.next_node;
		while (p) {
			size += 1;
			p = p->next_node;
		}
		return size;

	}

	[[nodiscard]] bool IsEmpty() const noexcept {
		return head_.next_node == nullptr;
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		if (!last) {
			last = head_.next_node;
		}
		++size_;
	}

	void PushBack(const Type& value) {
		if (last) {
			last->next_node = new Node(value, nullptr);
			last = last->next_node;
		}
		else {
			head_.next_node = new Node(value, head_.next_node);
			last = head_.next_node;
		}
	}

	void Clear() noexcept {
		while (head_.next_node) {
			Node* next = head_.next_node->next_node;
			delete head_.next_node;
			head_.next_node = next;
		}
		head_.next_node = nullptr;
		last = nullptr;
	}

	// Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
  // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] Iterator before_begin() noexcept {
		// Реализуйте самостоятельно
		return Iterator(&head_);
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		// Реализуйте самостоятельно
		//return ConstIterator(&head_);
		return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator before_begin() const noexcept {
		// Реализуйте самостоятельно
		//return ConstIterator(&head_);
		return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
	}

	/*
	 * Вставляет элемент value после элемента, на который указывает pos.
	 * Возвращает итератор на вставленный элемент
	 * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
	 */


	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		Node* current = pos.node_;
		if (current == &head_) {
			PushFront(value);
			return ConstIterator(head_.next_node);
		}
		else if (!current) {
			throw invalid_argument("Could not insert after end");
		}
		else {
			Node* new_node = new Node(value, current->next_node);
			current->next_node = new_node;
			++size_;
			return ConstIterator(current->next_node);
		}
	}




	void PopFront() noexcept {
		// Реализуйте метод самостоятельно
		Node* second_el_node = head_.next_node->next_node;  // нахожу второй элемент
		delete head_.next_node;   // удаляю первый 
		head_.next_node = second_el_node; // второй становится первым 
	}


	/*
	 * Удаляет элемент, следующий за pos.
	 * Возвращает итератор на элемент, следующий за удалённым
	 */

	Iterator EraseAfter(ConstIterator pos) noexcept {
		// Заглушка. Реализуйте метод самостоятельно
		Node* current = pos.node_;
		if (current == &head_) {
			PopFront();
			return ConstIterator(head_.next_node);
		}
		else if (!current || !current->next_node) {
			return cend();
		}
		else {
			Node* tmp = current->next_node->next_node;
			delete current->next_node;
			--size_;
			current->next_node = tmp;
			return ConstIterator(current->next_node);  // поскольку current->next_node удален, то новый (после сдвига влева в следствие уданиея элемента current->next_node) current->next_node это
			//элемент который был следом , справа за удаленным 
		}
	}


private:
	// Фиктивный узел, используется для вставки "перед первым элементом"
	Node head_;
	size_t size_;
};


template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs.GetSize() == rhs.GetSize() &&
		equal(lhs.cbegin(), lhs.cend(), rhs.cbegin())
		);
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs.GetSize() != rhs.GetSize() ||
		!equal(lhs.cbegin(), lhs.cend(), rhs.cbegin())
		);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return lexicographical_compare(lhs.cbegin(), lhs.cend(),
		rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs < rhs || lhs == rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !lexicographical_compare(lhs.cbegin(), lhs.cend(),
		rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs < rhs);
}
