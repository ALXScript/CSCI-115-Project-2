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
		while (p->next = !nullptr)
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
		Node* p = head;
		while (p->next != nullptr) {
			p = p->next;
		}
		p->next = nLL;
	}
}

bool MLinkList::isWall(int a, int b) {
	Node * p = head;
	while (p->next != null) {
		if (p->data == a) {
			return p->isZero(b);
		}
		else p = p->next;
	}
	if (p->data == a) {
		return p->isZero(b);
	}

}