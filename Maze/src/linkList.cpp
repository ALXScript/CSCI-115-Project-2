#include "linkList.h"
#include <iostream>

using namespace std;
//constructor
linkList::linkList(){

    root = nullptr;
    x = 0;
    y = 0;
    n = 2;
}
//constructor that creates with root Node*
linkList::linkList(Node* point) {

	root = point;
	x = point-> a;
	y = point-> b;
	n = 1;
	nextLL = nullptr;

}

//adds node to validPts
void linkList::addNode(Node* nn) { //checked and updated

	if (root == nullptr) {
		root = nn;
		n++;

	}

	else {
		Node * p = root;

		while (p->next != nullptr)
		{
			p = p->next;
		}

		p->next = nn;
		n++;

	}
}

//for deleting nodes
Node* linkList::delNode(Node* dn){

    Node * p = root;

    if (p==dn){
        root= root->next;
        p->next = nullptr;
        n--;
        return p;
    }

    while (p->next!= dn || p!=nullptr){
            p = p->next;
    }

    if (p->next==dn) {
        Node* temp = p->next;
        p-> next = temp -> next;
        temp -> next = nullptr;
        n--;
        return temp;
    }

    else return nullptr;

}

// calculates weight of current linkList
int linkList::listWeight(){

    int ans = 0;
    Node* p = root;

    while (p!= nullptr){
        ans = ans + p->weight;
        p = p->next;
    }

    sumEdges = ans;
    return ans;

}

// returns node in valid points
Node* linkList::lookup(int x, int y){

    Node* p = root;

    while (p!=nullptr){
        if (p->a == x && p->b == y)
            return p;
        else p = p->next;

    }
    return p;


}

void linkList::updateNeighbors(Node* srch, Node* newNeighbor){

    Node* p = root;
    while(p!=srch){
        p = p->next;
    }
    p->updateNeighbors(newNeighbor);

}

Node** linkList::retNeighbors(Node* srch){
    Node* p = root;

    while(p!=srch){
        p = p->next;
    }

    return p->neighbors;
}

void linkList::printList(){
    Node* p = root;
    while(p!= nullptr){
        cout<<"Current List is for point: ("<< p->a << ", "<< p->b<< "). Whose Neighbors are: ";
        p->displayN();
        p=p->next;

    }

}

//constructor for shortest path info
 minHeap::minHeap(){

    head = nullptr;

 }

 //adds node to minHeap
 void minHeap::addHeapNode(Node* vert){

    minHeapNode* nHN = new minHeapNode(vert);
    //cout<<"ADDED NODE: "<< nHN->vertex->a<<", "<< nHN->vertex->b<< " TO MIN HEAP"<< endl;

    if (head == nullptr){
        head = nHN;
    }

    else {
        minHeapNode* p = head;

        while(p->next != nullptr){
            p = p->next;
        }

        p->next= nHN;
    }

 }

 //returns minHeapNode
 minHeapNode* minHeap::retPtr (int srcha, int srchb){

    minHeapNode* p = head;

    while (p!= nullptr){

        int curra =p->vertex->a;
        int currb =p->vertex->b;
        if (curra == srcha && currb== srchb){
            return p;
        }
        else{

                p = p->next;}
    }
 }

 //returns true of minHeapNode->Node* is contained in array
 bool minHeap::isMember(Node * point, Node** arr, int V){

    for (int i = 0; i < V; i++){
        if (point == arr[i]) return true;
    }

    return false;
 }

 //removes Node* from array
 void minHeap::remArray(Node** arr, Node* point){

   int i = 0;

   while(arr[i] != point){
    i++;
   }

   arr[i]= nullptr;
 }

 //adds Node* to array
 void minHeap::addArray(Node** arr, Node* point, int &c){

    arr[c] = point;
 }

 bool minHeap::isEmpty(Node** arr, int sizeValPts){
    for(int i = 0; i< sizeValPts; i++){
        if (arr[i]!= nullptr) return false;
    }
    return true;

 }

 //returns closest vertex to argument only if it is closest (distSrc is least) and only if it has not been marked visited
 minHeapNode* minHeap::retClosestPtr(Node** visited, int V){
    int mini= 999999;
    minHeapNode* temp = head;
    minHeapNode* toBeRet = nullptr;

    while(temp!=nullptr){

        if (temp->distSrc <= mini && (!isMember(temp->vertex, visited, V))){
                    toBeRet = temp;
                    mini= temp->distSrc;}

        temp= temp->next;
    }
        return toBeRet;
 }

//most of the work for shortest path, works recursively by updating minHeap with info
 void minHeap::updateInfo(minHeapNode* t, linkList* adjList, Node** visited, int V, Node* start, int &c){
    if(c==V) return;

    Node* currNode = t->vertex;
    Node** currNodeNeighbors = adjList->retNeighbors(currNode);

    int tempDist = t->distSrc;
    int i = 0;

        while ( (currNodeNeighbors[i]!= nullptr) && i<4){

                if (isMember(currNodeNeighbors[i], visited, V)){
                i++;}

                else{

                    minHeapNode* tempMHN = retPtr(currNodeNeighbors[i]->a, currNodeNeighbors[i]->b);

                    if(tempMHN->distSrc > (tempDist + currNodeNeighbors[i]->weight)){
                        updatePtr(tempMHN, currNode, tempDist + currNodeNeighbors[i]-> weight);

                        }
                    i++;
                }

        }



    addArray(visited, t->vertex, c);
    c++;
    updateInfo(retClosestPtr(visited, V), adjList, visited, V, start, c);
 }

 //accepts the pointers for the Node of start and dest. searches through minHeap and traces back from dest to the Node after start. this returns the next location to be moved to
 Node* minHeap::nextPos(Node* start, Node * dest){

     minHeapNode* destination = retPtr(dest->a, dest->b);


     while(destination->prev!= start){

        destination = retPtr(destination->prev->a, destination->prev->b);

     }
     Node* nextStep = destination->vertex;

     return nextStep;

 }

 void minHeap::updatePtr(minHeapNode* &ex, Node* pred, int dist){
    ex->distSrc = dist;
    ex->prev = pred;

 }

 void minHeap::delHeap(minHeapNode* p){
    if (p->next != nullptr){
        delHeap(p->next);
    }
    delete p;
 }

 void minHeap::cleanArray(Node** arr, int V){
    for (int i = 0; i<V; i++){
        if (arr[i] != nullptr) arr[i]=nullptr;
    }

 }

 minHeap::~minHeap(){
    minHeapNode* current = head;
    while( current != nullptr ) {
        minHeapNode* next = current->next;
        delete current;
        current = next;
    }
    head= nullptr;
    cout<<"Deconstructor"<<endl;
 }

