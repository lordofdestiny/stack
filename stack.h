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
		template<typename structType>
		struct stack_node {
			using StackValueType = typename structType::ValueType;
			using StackValuePointerType = StackValueType*;
			using StackValueReferenceType = StackValueType&;
		public:
			StackValueType value;
			stack_node* next;
			stack_node() = default;
			template<class... _Valty>
			stack_node(_Valty&&... _Val)
				: value(std::forward<_Valty>(_Val)...), next(nullptr) {}
			stack_node(stack_node const& other) = default;
			stack_node(stack_node&& rhs) : value(rhs.value), next(rhs.next) {
				rhs.next = nullptr;
			}
		};
	public:
		using ValueType = Type;
		using PointerType = Type*;
		using ReferenceType = Type&;
		using NodeType = stack_node<stack<ValueType>>;
		using NodePointerType = NodeType*;
		using NodeReferenceType = NodeType&;
	public:
		stack() = default;
		stack(std::initializer_list<ValueType> il) {
			for (auto i = il.begin(); i != il.end(); i++) {
				this->emplace(*i);
			}
		}
		stack(stack const& other)
			: _size(other._size),
			data(stack::copyResoruces(other.data)) {};
		stack(stack&& other) noexcept :
			_size(std::exchange(other._size, 0)),
			data(std::exchange(other.data, nullptr)) {}
		virtual ~stack() {
			stack::freeResources(this->data);
		}
		stack& operator=(stack const& other) {
			if (*this != other) {
				stack::freeResources(this->data);
				this->_size = other._size;
				this->data = stack::copyResoruces(other->data);
			}
			return *this;
		}
		stack& operator=(stack&& other) {
			assert((*this != other));
			stack::freeResources(this->data);
			this->_size = std::exchange(other._size, 0);
			this->data = std::exchange(other.data, nullptr);
			return *this;
		};

		ValueType take() {
			ValueType tmp = std::move(this->data->value);
			this->pop();
			return tmp;
		}

		const ReferenceType top() const {
			if (data == nullptr) {
				throw ndb::stack_empty_error("Error while checking the top of an empty stack!");
			}
			return this->data->value;
		}

		stack& pop() throw(...) {
			if (data == nullptr) {
				throw ndb::stack_empty_error("Error while attempting to pop an empty stack!");
			}
			NodePointerType keep = this->data;
			this->data = this->data->next;
			delete keep;
			_size--;
			return *this;
		};

		stack& push(const ReferenceType value) throw(...) {
			return this->emplace(value);
		}

		stack& push(ValueType&& value) throw(...) {
			return this->emplace(std::move(value));
		}
		bool empty() const {
			return this->data == nullptr;
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
		template<class... _Valty>
		stack& emplace(_Valty&&... _Val) throw(...) {
			return perfect_empace(std::forward<_Valty>(_Val)...);
		}

	private:
		template<class... _Valty>
		stack& perfect_empace(_Valty&&... _Val) throw(...) {
			NodePointerType node = new NodeType(std::forward<_Valty>(_Val)...);
			node->next = this->data;
			this->data = node;
			++_size;
			return *this;
		}
		static void freeResources(NodePointerType top) {
			while (top != nullptr) {
				NodePointerType ptr = top->next;
				delete top;
				top = ptr;
			}
		}
		static NodePointerType copyResoruces(NodePointerType top) {
			NodePointerType data = nullptr;
			NodePointerType end = nullptr;
			while (top != nullptr) {
				NodePointerType node = new NodeType(*top);
				if (end == nullptr) {
					data = node;
				}
				else {
					end->next = node;
				}
				end = node;
				top = top->next;
			}
			return data;
		}
		NodePointerType data = nullptr;
		std::size_t _size = 0;
	};
}

