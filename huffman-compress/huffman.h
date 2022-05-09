#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include "heap.h"

template<class T>
struct HuffmanTreeNode {
	T weight;
	HuffmanTreeNode<T> *lson;
	HuffmanTreeNode<T> *rson;
	HuffmanTreeNode<T> *fa;
	HuffmanTreeNode(const T& w = T())
		:weight(w)
		,lson(nullptr)
		,rson(nullptr)
		,fa(nullptr)
	{}
	HuffmanTreeNode operator + (const HuffmanTreeNode& oo)const {
		return HuffmanTreeNode(weight + oo.weight);
	}
	HuffmanTreeNode operator != (const HuffmanTreeNode& oo)const {
		return weight != oo.weight;
	}
	HuffmanTreeNode operator < (const HuffmanTreeNode& oo)const {
		return weight < oo.weight;
	}
};

template<class T>
class HuffmanTree {
	typedef HuffmanTreeNode<T> Node;
protected:
	Node *root;
protected:
	Node *CreateHuffmanTree(const T* a, size_t size) {
		struct NodeLess {
			bool operator()(Node *pp, Node *qq)const {
				return pp->weight < qq->weight;
			}
		};
		Heap<Node *, NodeLess> MinHeap;
		for(size_t i=0;i<size;i++) {
			Node *tmp = new Node(a[i]);
			MinHeap.Push(tmp);
		}
		cout << "---- 建立优先队列 ----" << endl;
		while (MinHeap.Size() > 1) {
			Node *pp = MinHeap.Top();
			MinHeap.Pop();
			Node *qq = MinHeap.Top();
			MinHeap.Pop();
			Node *fa = new Node(pp->weight + qq->weight);
			fa->lson = pp;
			fa->rson = qq;
			pp->fa = fa;
			qq->fa = fa;
			MinHeap.Push(fa);
		}
		return MinHeap.Top();
	}
	Node *CreateHuffmanTree(const T* a, size_t size, const T& invalid) {
		struct NodeLess {
			bool operator()(Node *pp, Node *qq)const {
				return pp->weight < qq->weight;
			}
		};
		Heap<Node *, NodeLess> MinHeap;
		for (size_t i = 0; i < size;i++) {
			if (a[i] != invalid) {
				Node *tmp = new Node(a[i]);
				MinHeap.Push(tmp);
			}
		}
		while (MinHeap.Size() > 1) {
			Node *pp = MinHeap.Top();
			MinHeap.Pop();
			Node *qq = MinHeap.Top();
			MinHeap.Pop();
			Node *fa = new Node(pp->weight + qq->weight);
			fa->lson = pp;
			fa->rson = qq;
			pp->fa = fa;
			qq->fa = fa;
			MinHeap.Push(fa);
		}
		return MinHeap.Top();
	}
	void Destroy(Node *&root) {
		if (root = nullptr)
			return;
		Node *cur = root;
		if (cur) {
			Destroy(cur->lson);
			Destroy(cur->rson);
			delete cur;
			cur = nullptr;
			return;
		}
	}
public:
	HuffmanTree()
		:root(nullptr)
	{}
	HuffmanTree(const T*a, size_t size)
		:root(nullptr)
	{
		root = CreateHuffmanTree(a, size);
	}
	HuffmanTree(const T*a, size_t size, const T& invalid) {
		root = CreateHuffmanTree(a, size, invalid);
	}
	Node* GetRoot() {
		return root;
	}
	~HuffmanTree() {
		Destroy(root);
	}
};

//void TestHuffmanTree() {
//	int a[] = { 0,1,2,3,4,5,6,7,8,9 };
//	int size = sizeof(a) / sizeof(a[0]);
//	HuffmanTree<int> ht(a, size);
//}