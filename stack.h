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
	class stack
	{
	public:
		using ValueType = Type;
		using PointerType = Type*;
		using ReferenceType = Type&;
	private:
		struct stack_node {
			ValueType value;
			stack_node* next;
			stack_node() = default;
			stack_node(stack_node const& other) = default;
			stack_node(stack_node&& rhs)
				: value(rhs.value),
				next(std::exchange(rhs.next, nullptr)) {}
		};
		using NodeType = stack_node;
		using NodePointerType = NodeType*;
		using NodeReferenceType = NodeType&;
	public:
		stack() = default;
		stack(std::initializer_list<ValueType> il) {
			for (const auto& i : il) {
				emplace(i);
			}
		}
		stack(stack const& other)
			: _size(other._size),
			_data(stack::copyResoruces(other._data)) {};
		stack(stack&& other) noexcept :
			_size(std::exchange(other._size, 0)),
			_data(std::exchange(other._data, nullptr)) {}
		virtual ~stack() {
			freeResources();
		}
		stack& operator=(stack const& other) {
			if (*this != other) {
				freeResources();
				_size = other._size;
				_data = stack::copyResoruces(other->_data);
			}
			return *this;
		}
		stack& operator=(stack&& other) {
			assert((*this != other));
			freeResources();
			_size = std::exchange(other._size, 0);
			_data = std::exchange(other._data, nullptr);
			return *this;
		};

		ValueType take() noexcept(false){
			if (empty()) {
				throw ndb::stack_empty_error("Error while taking from an empty stack!");
			}
			ValueType tmp = std::move(_data->value);
			pop();
			return tmp;
		}

		const ReferenceType top() const noexcept(false) {
			if (empty()) {
				throw ndb::stack_empty_error("Error while checking the top of an empty stack!");
			}
			return _data->value;
		}

		stack& pop() noexcept(false) {
			if (empty()) {
				throw ndb::stack_empty_error("Error while attempting to pop an empty stack!");
			}
			freeTop();
			return *this;
		};

		stack& push(const ReferenceType value) noexcept(false) {
			return emplace(value);
		}

		stack& push(ValueType&& value) noexcept(false) {
			return emplace(std::move(value));
		}
		bool empty() const {
			return _data == nullptr;
		}
		std::size_t size() const {
			return _size;
		}
		std::vector<ValueType> take_all() {
			std::vector<ValueType> vec;
			vec.reserve(_size);
			while (!empty()) {
				vec.push_back(std::move(take()));
			}
			return vec;
		}
		template<class... ValType>
		stack& emplace(ValType&&... args) noexcept(false) {
			return perfect_empace(std::forward<ValType>(args)...);
		}

	private:
		NodePointerType _data = nullptr;
		std::size_t _size = 0;
		template<class... ValType>
		stack& perfect_empace(ValType&&... args) noexcept(false) {
			NodePointerType node = new NodeType();
			new(&node->value) ValueType(std::forward<ValType>(args)...);
			node->next = _data;
			_data = node;
			++_size;
			return *this;
		}
		void freeTop() {
			NodePointerType ptr = _data;
			_data = _data->next;
			ptr->value.~ValueType();
			delete ptr;
			--_size;
		}
		void freeResources() {
			while (_data != nullptr) {
				freeTop();
			}
		}
		static NodePointerType copyResoruces(NodePointerType top) {
			NodePointerType _data = nullptr, end = nullptr;
			while (top != nullptr) {
				NodePointerType node = new NodeType(*top);
				end = (end == nullptr ? _data : end->next ) = node;
				top = top->next;
			}
			return _data;
		}
	};
}

