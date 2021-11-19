#include <algorithm>
#include <iostream>
#include <vector>
#include "stack.h"

struct X {
	int x;
	X(int x = 0) : x(x) {}
	X(X const& other) = default;
	X(X&& other) noexcept : x(std::exchange(other.x, 0)) {}
	friend std::ostream& operator << (std::ostream& os, const X& x) {
		return os << "X=" << x.x;
	}
};

template<typename Type>
std::ostream& operator<<(std::ostream& os, std::vector<Type> const& vec) {
	std::copy(vec.cbegin(), vec.cend(), std::ostream_iterator<Type>(os, " "));
	return os;
}

int main() {
	ndb::stack<X> s = { 1,2,3,4,5,6 };
	auto s2 = s;
	auto vec = s.take_all();
	std::cout << vec << std::endl;
	for (auto& x : vec) {
		s2.emplace(x);
	}
	for (int i = 0; i < 3; i++) {
		std::cout << s2.take() << std::endl;
	}
	auto vec2 = s2.take_all();
	std::cout << vec2 << std::endl;
}