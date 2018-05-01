#include "linkList.h"

linkList::linkList(int a) {
	root = nullptr;
	data = a;
}
void linkList::addNode(int a) {
	Node* nn = new Node(a);
	if (root == nullptr) {
		root = nn;
	}
	else {
		Node * p = root;
		while (p->next != nullptr)
		{
			p = p->next;
		}
		p->next = nn;
	}
}
bool linkList::isZero(int a) { // checks if data is == to argument and returns true if the place on graph is a wall
	Node* p = root;
	while (p->next != nullptr) {
		if (p->data == a) return false;
		else p = p->next;
	}
	if (p->data == a) return false;
	else return true;
}
MLinkList::MLinkList() {
	head = nullptr;
}
void MLinkList::addLinkList(int a) {
	linkList* nLL = new linkList(a);
	if (head == nullptr) {
		head = nLL;
	}
	else {
		linkList* p = head;
		while (p->nextLL != nullptr) {
			p = p->nextLL;
		}
		p->nextLL = nLL;
	}
}

bool MLinkList::isWall(int a, int b) {
	linkList * p = head;
	while (p->nextLL != nullptr) {
		if (p->data == a) {
			return p->isZero(b);
		}
		else p = p->nextLL;
	}
	if (p->data == a) {
		return p->isZero(b);
	}

}
