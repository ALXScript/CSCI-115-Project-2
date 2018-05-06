#ifndef LINK_LIST_H
#define LINK_LIST_H

typedef struct Node{
	Node(int a, int b) {
		 a = a; //number for tuple (a,b)
		 b = b; // number for tuple (a,b)
         Node* next = nullptr;
         weight = 1;
	}
	Node (Node* n){
        a = n-> a;
        b = n-> b;
        next = n-> next;
        weight = n->weight;
	}
	int a;
	int b;
	int weight;
	Node* next;
} Node;

class linkList{

public:
        Node* root;
		linkList* nextLL;
		int x;
		int y;
		int n;
		int sumEdges;

		linkList(Node* rt);
		linkList();
		int listWeight();
		void addNode(Node* nn);
		Node* delNode(Node* dn);


};

class MLinkList {
public:
		MLinkList();
		linkList* addLinkList(Node* point);
		linkList* retLL(Node* point);
		bool isWall(int a, int b);
        linkList* head;
};

typedef struct minHeapNode{
    Node* vertex;
    Node* prev;
    int distSrc;
    minHeapNode* next;
    minHeapNode(Node* vert){
        vertex = vert;
        prev = nullptr;
        distSrc = 999999;
        next = nullptr;
    }

}minHeapNode;

class minHeap{
public:
    minHeapNode* head;
    minHeap();
    void addHeapNode(Node* vert);
    minHeapNode* retPtr(Node* srch);
    minHeapNode* retClosestPtr(Node* pred, Node** arr, int V);
    bool isMember(Node* point, Node** arr, int V);
    void updateInfo(minHeapNode* t, MLinkList* adjList, Node** visited, Node** unvisited, int V);
    void remArray(Node** arr, Node* point);
    void addArray(Node** arr, Node* point);
    Node* nextPos(Node* start, Node* dest);
};

#endif

