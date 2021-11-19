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

struct Node {
	Node* row, * col;
	int val;
	int r, c;
	Node(int r, int c, int val = 0) :
		row(this), col(this), val(val), r(r), c(c) {}
};

int main() {
	ndb::stack<X> s = { 1,2,3,4,5,6 };
	s.emplace(1);
	std::cout << s.take_all() << std::endl;
	int N = 6;
	int M = 5;
	Node** Ra = new Node * [N + 1];
	Node** Ca = new Node * [M + 1];

	for (int i = 0; i < N + 1; i++) {
		Ra[i] = new Node(0, 0);
	}
	for (int i = 0; i < N + 1; i++) {
		Ca[i] = new Node(0, 0);
	}

	int R[8], C[8], V[8];

	Node* a = new Node(1, 1, 1);
	Node* b = new Node(1, 2, 2);
	Node* c = new Node(1, 3, 3);
	Node* d = new Node(3, 3, 4);
	Node* e = new Node(5, 3, 5);
	Node* f = new Node(5, 4, 6);

	Ra[1]->row = a;
	a->row = b;
	b->row = c;
	c->row = Ra[1];

	Ra[3]->row = d;
	d->row = Ra[3];

	Ra[5]->row = e;
	e->row = f;
	f->row = Ra[5];

	Ca[1]->col = a;
	a->col = Ca[1];

	Ca[2]->col = b;
	b->col = Ca[2];

	Ca[3]->col = c;
	c->col = d;
	d->col = e;
	e->col = Ca[3];

	Ca[4]->col = f;
	f->col = Ca[4];

	int cnt = 1;
	for (int i = 1; i <= N; i++) {
		Node* curr = Ra[i]->row;
		while (curr != Ra[i]) {
			R[i] = cnt;
			C[cnt] = curr->c;
			V[cnt] = curr->val;
			cnt += 1;
			curr = curr->row;
		}
	}
	R[N + 1] = cnt + 1;

	for (int i = 0; i < 8; i++) {
		std::cout << R[i] << ' ' << C[i] << ' ' << V[i] << std::endl;
	}
}