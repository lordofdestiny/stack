#include <algorithm>
#include <iostream>
#include <vector>
#include "stack.h"

struct X {
	int x;
	X(int x = 0) : x(x) {}
	X(X const& other) = default;
	X(X&& other) noexcept: x(std::exchange(other.x,0)){}
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
	ndb::stack<X> s = { 1,2,3,4,5 };
	s.push(0).push({ -1 });
	s.emplace(-2);
	std::cout << s.take_all();
}