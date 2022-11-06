#pragma once
#include <cstddef>
#include <cassert>
#include <vector>
#include <utility>
#include <exception>

namespace ndb {
	struct stack_empty_error : public std::exception {
		stack_empty_error(const char* msg) noexcept
			:std::exception(msg) {}
	};

	template<typename Type>
	class stack final
	{
	public:
		using ValueType = Type;
		using Pointer = Type*;
		using Reference = Type&;
		using ConstReference = Type const&;
		using RValueReference = Type&&;
		using StackSizeType = std::size_t;
		using VectorType = std::vector<ValueType>;
	private:
		struct stack_node {
			ValueType value;
			stack_node* next = nullptr;
			template<class... ValType>
			stack_node(ValType&&... args) 
				: value(std::forward<ValType>(args)...) {}
			stack_node(ConstReference value) 
				: value(value) { }
			stack_node(RValueReference value) 
				: value(std::move(value)) {}

			stack_node(stack_node const& other) 
				: value(other.value) {}
			stack_node(stack_node&& rhs)
				: value(std::move(rhs.value)),
				next(std::exchange(rhs.next, nullptr)) {}
		};
		using NodeType = stack_node;
		using NodePointer = NodeType*;
		using NodeReference = NodeType&;
	public:
		stack() = default;
		stack(std::initializer_list<ValueType> il) {
			for (ConstReference i : il) emplace(i);
		}
		stack(stack const& other) :
			_size(other._size),
			_data(other.copyResources()) {};
		stack(stack&& other) noexcept :
			_size(std::exchange(other._size, 0)),
			_data(std::exchange(other._data, nullptr)) {}
		~stack() {
			freeResources();
		}
		stack& operator=(stack const& other) {
			if (this != &other) {
				auto newData = other.copyResources();
				freeResources();
				_size = other._size;
				_data = newData;
			}
			return *this;
		}
		stack& operator=(stack&& other) noexcept {
			if (this != &other) {
				freeResources();
				_size = std::exchange(other._size, 0);
				_data = std::exchange(other._data, nullptr);
			}
			return *this;
		};

		ValueType take() noexcept(false) {
			ValueType tmp = std::move(_data->value);
			pop();
			return tmp;
		}

		ConstReference top() const noexcept(false) {
			if (empty()) {
				throw ndb::stack_empty_error("Error while checking the top of an empty stack!");
			}
			return _data->value;
		}

		stack& pop() noexcept(false) {
			if (empty()) {
				throw ndb::stack_empty_error("Error while attempting to pop an empty stack!");
			}
			removeTop();
			return *this;
		};

		stack& push(const Reference value) noexcept(false) {
			return emplace(value);
		}

		stack& push(ValueType&& value) noexcept(false) {
			return emplace(std::move(value));
		}
		bool empty() const {
			return _data == nullptr;
		}
		StackSizeType size() const {
			return _size;
		}
		stack& clear() {
			freeResources();
		}
		VectorType take_all() {
			VectorType vec;
			vec.reserve(_size);
			while (!empty()) {
				vec.emplace_back(std::move(this->_data->value));
				removeTop();
			}
			return vec;
		}

		template<class... ValType>
		stack& emplace(ValType&&... args) noexcept(false) {
			return perfect_emplace(std::forward<ValType>(args)...);
		}

	private:
		template<class... ValType>
		stack& perfect_emplace(ValType&&... args) noexcept(false) {
			auto node = new NodeType(std::forward<ValType>(args)...);
			node->next = std::exchange(_data, node);
			++_size;
			return *this;
		}
		void removeTop() {
			NodePointer ptr = std::exchange(_data, _data->next);
			ptr->value.~ValueType();
			delete ptr;
			--_size;
		}
		void freeResources() {
			while (_data != nullptr) removeTop();
		}
		NodePointer copyResources() const {
			NodePointer tmp = this->_data;
			NodePointer head = nullptr, tail = nullptr;
			while (tmp != nullptr) {
				NodePointer node = new NodeType(*tmp);
				tail = (tail == nullptr ? head : tail->next) = node;
				tmp = tmp->next;
			}
			return head;
		}
		NodePointer _data = nullptr;
		StackSizeType _size = 0;
	};
}

