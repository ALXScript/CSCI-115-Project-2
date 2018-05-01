#ifndef LINK_LIST_H
#define LINK_LIST_H

typedef struct Node{
	Node(int a) {
		int data = a;
		Node* next = nullptr;
	}
	int data;
	Node* next;
} Node;

class linkList {

private:
		Node* root;
		//linkList* nextLL;
		//int data;

public:
		linkList(int a);
		void addNode(int a);
		bool isZero(int a);
		linkList* nextLL;
		int data;
};

class MLinkList {
private:

		linkList* head;

public:
		MLinkList();
		void addLinkList(int a);
		bool isWall(int a, int b);

};

#endif

