#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
#include <chrono>
#include <iomanip>
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

class Timer {
public:
	using clock = std::chrono::high_resolution_clock;
	bool called = false;
	Timer() {
		startTime = clock::now();
	}
	explicit Timer(const char name[]) {
		startTime = std::chrono::high_resolution_clock::now();
		this->name = std::string(name);
		named = true;
	}
	~Timer() {
		if (called) return;
		called = true;
		auto endTime = clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
		auto seconds = duration.count() / 1e9;

		if (named) {
			std::cout << "Timer: " << name << "\n";
		}
		auto oldprec = std::cout.precision();
		std::cout << "Time of execution: " << std::setprecision(5) << std::fixed << seconds << "seconds.\n";
		std::cout << std::defaultfloat << std::setprecision(oldprec);
	}
private:
	clock::time_point startTime;
	std::string name;
	bool named = false;
};

int main() {
	std::cout << "std::stack\n";
	{
		Timer t1("Allocation 1");
		std::stack<X> s;
		t1.~Timer();
		Timer t2("Pushing 1");
		for (int i = 0; i < 1000000; i++) s.push(i);
		t2.~Timer();
		Timer t3("Poping 1 ");
		while (!s.empty()) s.pop();
	}
	std::cout << std::string(40, '-') << std::endl;
	std::cout << "nbd::stack\n";
	{
		Timer t1("Allocation 1");
		ndb::stack<X> s;
		t1.~Timer();
		Timer t2("Pushing 2");
		for (int i = 0; i < 1000000; i++) s.push(i);
		t2.~Timer();
		Timer t3("Poping 2");
		while (!s.empty()) s.pop();
	}
}