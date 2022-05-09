#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

/*
一个手写堆的板子
*/

template<class T>
struct Small {
	bool operator()(const T& pp, const T& qq) {
		return pp < qq;
	}
};

template<class T>
struct Big {
	bool operator()(const T& pp, const T& qq) {
		return pp > qq;
	}
};

template<class T, class Compare = Big<T> >
class Heap {
protected:
	vector<T> a;
protected:
	void AdjustDown(int u) {
		int fa = u;
		unsigned int child = (u << 1) + 1;
		while (child < a.size()) {
			Compare com;
			if (child + 1 < a.size() && com(a[child + 1], a[child]))
				child++;
			if (com(a[child], a[fa])) {
				swap(a[child], a[fa]);
				fa = child;
				child = (fa << 1) + 1;
			}
			else
				break;
		}
	}
	void AdjustUp(int child) {
		while (child > 0) {
			int fa = (child - 1) >> 1;
			Compare com;
			if (com(a[child], a[fa])) {
				swap(a[child], a[fa]);
				child = fa;
			}
			else
				break;
		}
	}
public:
	Heap()
	{}
	/*Heap(const T *a1, int size) {
		assert(a1);
		a.reserve(size);
		for (int i = 0; i < size; i++)
			a.push_back(a1[i]);
		for (int i = (size - 2) >> 1; i >= 0; i--)
			AdjustDown(i);
	}*/
	void Push(const T& oo) {
		a.push_back(oo);
		AdjustUp(a.size() - 1);
	}
	void Pop() {
		assert(!a.empty());
		swap(a[0], a[a.size() - 1]);
		a.pop_back();
		AdjustDown(0);
	}
	size_t Size() {
		return a.size();
	}
	bool Empty() {
		return a.empty();
	}
	const T & Top()const {
		assert(!a.empty());
		return a[0];
	}
	void Display() {
		for (auto oo : a)
			cout << oo << " ";
		puts("");
	}
};