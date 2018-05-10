#ifndef LINK_LIST_H
#define LINK_LIST_H
#include <iostream>
using namespace std;
typedef struct Node{

	Node(int x, int y) {
		 a = x;                     //number for tuple (a,b)
		 b = y;                     // number for tuple (a,b)
         next = nullptr;
         weight = 1;
         initNeighbors();
	}

	Node (Node* n){                 //constructor with another node to create copy so copy can be deleted
        a = n-> a;
        b = n-> b;
        next = n-> next;
        weight = n->weight;
	}

	int a;                          // x value of node
	int b;                          // y value of node
	int weight;                     // weight of node
	Node* next;                     //next node
	Node* neighbors[4];

	void initNeighbors(){
        for (int i= 0; i <4; i++){
            neighbors[i]= nullptr;
        }
	}

	void updateNeighbors(Node* newNeighbor){
        int i = 0;
        while(neighbors[i] != nullptr){
            i++;
        }
        neighbors[i]= newNeighbor;
	}

	bool checkNeighbor(Node* srch){
        for (int i = 0; i < 4; i++){
            if (srch == neighbors[i]) return true;
        }
        return false;
	}

	void displayN(){
        int i=0;
        while(neighbors[i]!= nullptr && i < 4){
            cout<<"(" <<neighbors[i]->a <<" ,"<<neighbors[i]-> b <<") ";
            i++;
        }
        cout<<endl;

	}
} Node;

//stores the nodes adjacent to root
class linkList{

public:
        Node* root;                 // curr node
		linkList* nextLL;           // next list in adj list
		int x;                      // x position of currNode
		int y;                      // y position of currNode
		int n;                      // number of items in list
		int sumEdges;               //sum of edges

		linkList(Node* rt);         // constructor with node
		linkList();                 // constructor without node; empty list
		int listWeight();           //calculates weight of list;
		void addNode(Node* nn);     // adds node
		Node* delNode(Node* dn);    // deletes node
		Node* lookup(int x, int y); //finds node of corresponding x,y
		void updateNeighbors(Node* srch, Node* newNeighbor);
		Node** retNeighbors(Node* srch);
        void printList();
};



//Node for minHeap. it stores current Node*, distance from source, and previous node.
typedef struct minHeapNode{
    Node* vertex;                   //curr node
    Node* prev;                     // node that leads to current node in shortest known way
    int distSrc;                    // nodes away from source in graph
    minHeapNode* next;              // next stored minHeap Node
    minHeapNode(Node* vert){        // constructor with Node*
        vertex = vert;
        prev = nullptr;
        distSrc = 999999;
        next = nullptr;
    }

}minHeapNode;

//used to store info gathered by Dijkstras alg
class minHeap{
public:
    minHeapNode* head;                                                  //beginning of heap
    minHeap();                                                          //constructor of minHeap
    ~minHeap();                                                         //Destructor
    void delHeap(minHeapNode* p);                                       //function for use in destructor
    void addHeapNode(Node* vert);                                       //adds heap node
    minHeapNode* retPtr(int srcha, int srchb);                          // returns pointer of heapNode for corresponding Node*
    minHeapNode* retClosestPtr(Node** arr, int V);                      //returns closest ptr to current node
    bool isMember(Node* point, Node** arr, int V);                      // returns bool value for if pointer is member of array
    void updateInfo(minHeapNode* t, linkList* adjList, Node** visited, int V, Node* start, int &cn);           //updates info on in heap while dijkstras alg is running
    void remArray(Node** arr, Node* point);                             //removes Node* from array
    void addArray(Node** arr, Node* point, int &c);                     //adds pointer to an array of pointers
    bool isEmpty(Node** arr, int sizeValPts);                           //adds to array
    Node* nextPos(Node* start, Node* dest);                             //returns next position of enemy
    void updatePtr(minHeapNode* &ex, Node*prev, int dist);
    void cleanArray(Node** arr, int V);
};

#endif

