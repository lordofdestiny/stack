#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <chrono>
#include "stack.h"
#include "StackTester.h"
#include "Timer.h"

#define CONSTRUCTOR_DEBUG 0

struct X {
	double x;
	X(double x = 0) : x(x) {
#if CONSTRUCTOR_DEBUG
		std::cerr << "Constructed!\n";
#endif
	}
	X(X const& other) : x(other.x) {
#if CONSTRUCTOR_DEBUG
		std::cerr << "Made a copy!\n";
#endif
	};
	X(X&& other) noexcept : x(std::exchange(other.x, 0)) {
#if CONSTRUCTOR_DEBUG
		std::cerr << "Did a move!\n";
#endif
	}
	~X() {
#if CONSTRUCTOR_DEBUG
		std::cerr << "Destroyed!\n";
#endif
	}
	friend std::ostream& operator << (std::ostream& os, const X& x) {
		return os << "X=" << x.x;
	}
};

template<typename Type>
std::ostream& operator<<(std::ostream& os, std::vector<Type> const& vec) {
	std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<Type>(os, " "));
	return os;
}

static constexpr size_t iterations = 10'000'000;

int main() {
	using Utils::Timer;

	Utils::StackTester<std::stack<X, std::deque<X>>, iterations> stdstack_deque_test(true);
	Utils::StackTester<std::stack<X, std::vector<X>>, iterations> stdstack_vector_test(true);
	Utils::StackTester<std::stack<X, std::list<X>>, iterations> stdstack_list_test(true);
	Utils::StackTester<ndb::stack<X>, iterations> ndbstack_test(true);

}