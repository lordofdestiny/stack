#pragma once

#include <cstddef>
#include <new>
#include "Timer.h"


namespace Utils {
	template<class StackType, size_t IterationCount>
	class StackTester {
	public:
		StackTester(bool immidiate = false, std::ostream& os = std::cout) 
			: os(std::cout) {
			if (immidiate) {
				run();
			}
		}
		StackTester(StackTester const&) = delete;
		StackTester& operator=(StackTester const&) = delete;
		~StackTester() {
			if (stack != nullptr) {
				delete stack;
			}
		};
		void run() try {
			os << "Testing " << typeid(StackType).name() << "\n";
			allocate();
			populate();
			empty();
			deallocate();
			os << std::string(40, '-') << '\n';
		}
		catch (...) {};
	private:
		void allocate() try {
			Timer t("Allocating");
			stack = new StackType();
		}
		catch (std::bad_alloc const& e) {
			os << "Allocation failed: " << e.what() << std::endl;
			throw;
		}
		void populate() {
			Timer t("Populating");
			for (int i = 0; i < IterationCount; i++) stack->push(i);
		}
		void empty() {
			Timer t("Emptying");
			while (!stack->empty()) stack->pop();
		}
		void deallocate() {
			Timer t("Deallocating");
			if (stack != nullptr) {
				delete stack;
				stack = nullptr;
			}
		}
	private:
		StackType* stack = nullptr;
		std::ostream& os;
	};

}