#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <chrono>
#include <iomanip>
#include "stack.h"
#include "Timer.h"

#define CONSTRUCTOR_DEBUG 0

struct X {
	double x;
	X(double x = 0) : x(x) { 
#if CONSTUCTOR_DEBUG
		std::cerr << "Constructed!\n";
#endif
	}
	X(X const& other) : x(other.x) {
#if CONSTUCTOR_DEBUG
		std::cerr << "Made a copy!\n";
#endif
	};
	X(X&& other) noexcept : x(std::exchange(other.x, 0)) {
#if CONSTUCTOR_DEBUG
		std::cerr << "Did a move!\n";
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

static auto iterations = 10'000'000;

int main() {
	using Utils::Timer;
	std::cout << "std::stack\n";
	{
		Timer t1("Allocation 1");
		std::stack<X> s;
		t1.stop();
		Timer t2("Pushing 1");
		for (int i = 0; i < iterations; i++) s.push(i);
		t2.stop();
		Timer t3("Poping 1 ");
		while (!s.empty()) s.pop();
		t3.stop();
	}
	std::cout << std::string(40, '-') << std::endl;

	std::cout << "nbd::stack\n";
	{
		Timer t1("Allocation 1");
		ndb::stack<X> s;
		t1.stop();
		Timer t2("Pushing 2");
		for (int i = 0; i < iterations; i++) s.push(i);
		t2.stop();
		Timer t3("Poping 2");
		while (!s.empty()) s.pop();
		t3.stop();
	}
}