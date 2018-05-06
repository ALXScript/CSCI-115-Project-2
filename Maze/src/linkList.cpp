#include "linkList.h"
linkList::linkList(){

    root = nullptr;
    x = 0;
    y = 0;
    n = 2;
}
linkList::linkList(Node* point) {

	root = point;
	x = point-> a;
	y = point-> b;
	n = 1;

}
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

MLinkList::MLinkList() {
	head = nullptr;
}

linkList* MLinkList::addLinkList(Node* point) { // checked and updated
	linkList* nLL = new linkList(point);
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
	return nLL;
}

bool MLinkList::isWall(int a, int b) {
    linkList* p = head;
    while(p!= nullptr){
        if (p->x == a && p-> y == b) return false;
        else return true;
    }

}

linkList* MLinkList::retLL(Node* point){
    linkList* p = head;
    while(p->root != point){
        p= p->nextLL;
    }
    if (p->root == point) return p;
    else return nullptr;
}
 minHeap::minHeap(){
    head = nullptr;

 }

 void minHeap::addHeapNode(Node* vert){

    minHeapNode* nHN = new minHeapNode(vert);
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

 minHeapNode* minHeap::retPtr (Node* srch){
    minHeapNode* p = head;
    while (p!= nullptr){
        if (p->vertex == srch) return p;
        p= p->next;
    }
    return nullptr;

 }
 bool minHeap::isMember(Node * point, Node** arr, int V){
    for (int i = 0; i < V; i++){
        if (point == arr[i]) return true;
    }
    return false;
 }

 void minHeap::remArray(Node** arr, Node* point){
   int i = 0;
   while(arr[i] != point){
    i++;
   }
   arr[i]= nullptr;
 }

 void minHeap::addArray(Node** arr, Node* point){
    int i = 0;
    while (arr[i] != 0){
        i++;
    }
    arr[i] = point;
 }

 //returns closest vertex to argument only if it is closest (distSrc is least) and only if it has not been marked visited
 minHeapNode* minHeap::retClosestPtr(Node* pred, Node** visited, int V){
    minHeapNode* temp = head;
    minHeapNode* toBeRet = nullptr;
    while(temp!=nullptr){
        if (temp->prev == pred && (!isMember(temp->vertex, visited, V))){
            if (toBeRet == nullptr || (toBeRet->distSrc > temp->distSrc)){
                toBeRet = temp;
            }
        }
        temp= temp->next;
    }
    return toBeRet;
 }


 void minHeap::updateInfo(minHeapNode* t, MLinkList* adjList, Node** visited, Node** unvisited, int V){
    if(isMember(t->vertex, visited, V) || t == nullptr) return;
    linkList* currLL = adjList->retLL(t->vertex);
    int tempDist = t->distSrc;
    Node* currNode = currLL->root;
    while (currNode->next!= nullptr){
        while (!isMember(currNode->next, visited, V)){
                minHeapNode* tempMHN = retPtr(currNode->next);
                if (tempMHN->distSrc > (tempDist + currNode->next->weight)){
                    tempMHN->distSrc= (tempDist + currNode->next->weight);
                    tempMHN->prev= currNode;
                }
        }
        currNode = currNode->next;
    }

    remArray(unvisited, t->vertex);
    addArray(visited, t->vertex);
    updateInfo(retClosestPtr(t->vertex, visited, V), adjList, visited, unvisited, V);
 }

 Node* minHeap::nextPos(Node* start, Node * dest){
     minHeapNode* destination = retPtr(dest);
     while(destination->prev!= start){
        destination = retPtr(destination->prev);
     }
     return destination->vertex;

 }

