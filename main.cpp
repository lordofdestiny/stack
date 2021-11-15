#include <iostream>
#include <vector>
#include "stack.h"

struct X {
	int x;
	X(int x = 0) : x(x) {}
	X(X const& other) = default;
	X(X&& other) noexcept: x(std::exchange(other.x,0)){}
	friend std::ostream& operator << (std::ostream& os, const X& x) {
		return os << x.x;
	}
};

int main() {
	ndb::stack<X> s = { 4,5,6 };
	s.push(1).push(2).push(3);
	while (!s.empty()) {
		std::cout << s.take() << " ";
	}
	std::cout << std::endl;
}