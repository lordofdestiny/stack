#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <forward_list>
#include <chrono>
#include <iomanip>
#include "stack.h"
#include "Timer.h"

struct X {
	int x;
	X(int x = 0) : x(x) { std::cerr << "Constructed!\n"; }
	X(X const& other) : x(x) { std::cerr << "Made a copy!\n"; };
	X(X&& other) noexcept : x(std::exchange(other.x, 0)) { std::cerr << "Did a move!\n"; }
	friend std::ostream& operator << (std::ostream& os, const X& x) {
		return os << "X=" << x.x;
	}
};

template<typename Type>
std::ostream& operator<<(std::ostream& os, std::vector<Type> const& vec) {
	std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<Type>(os, " "));
	return os;
}

static auto iterations = 10;

int main() {
	using Utils::Timer;
	/*std::cout << "std::stack\n";
	{
		Timer t1("Allocation 1");
		std::stack<X, std::list<X>> s;
		t1.stop();
		Timer t2("Pushing 1");
		for (int i = 0; i < iterations; i++) s.push(i);
		t2.stop();
		Timer t3("Poping 1 ");
		while (!s.empty()) s.pop();
		t3.stop();
	}
	std::cout << std::string(40, '-') << std::endl;*/

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