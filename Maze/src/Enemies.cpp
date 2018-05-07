/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include "Enemies.h"
#include <iostream>

using namespace std;

Timer *T1 = new Timer();                             // Set Timer for animation

Enemies::Enemies()
{
    //ctor
    enmLoc.x=0;
    enmLoc.y=0;

    xmax =1;
    ymax =0.25;
    xmin =0;
    ymin =0;
    up= down = left=right=false;
    live = true;

}

Enemies::~Enemies()
{
    //dtor
}

void Enemies::initEnm(int grid,int numFrames, char * FileName)
{
    gridSize = grid;
    frames = numFrames;
    xmax =1/(float)frames;
    T1->Start();
    t = (float)(2.0/grid)/frames;
    unitWidth = (float)2.0/gridSize;
    enmTex = TextureLoader(FileName);
}


void Enemies::drawEnemy()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,enmTex);


    glPushMatrix();
    animate();
    glTranslatef(enmLoc.x,enmLoc.y,0.0);

    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_POLYGON);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
     glEnd();
     glPopMatrix();
}

void Enemies::placeEnemy(int x, int y)
{
    unitWidth = 2.0/gridSize;
    x+=1;
    y+=1;
    enmLoc.x =  -1-unitWidth/2+(unitWidth)*x;
    enmLoc.y =  -1-unitWidth/2+(unitWidth)*y;
}

void Enemies::moveEnemy(string dir)
{
  if(moveDis<=0){
   if(dir=="up"){up=true; down=left=right=false;}
   else if(dir=="down"){down=true; up=left=right=false;}
   else if(dir=="left"){left=true; down=up=right=false;}
   else if(dir=="right"){right=true; down=left=up=false;}
   else{up=left=right=false;}
    }
}

void Enemies::animate()
{

if(moveDis < unitWidth)
{

if(T1->GetTicks()<1000)
   {

if(up)
   {
       if(enmLoc.y< 1-unitWidth/2)
        {
            enmLoc.y += t;
         }   moveDis +=t;

        xmin +=1/(float)frames;
        xmax +=1/(float)frames;
        ymin =0.0;
        ymax =0.25;
        if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }
    }

  else if(down)
   {
         if(enmLoc.y > -1+unitWidth/2)
           {
              enmLoc.y -= t;
           }  moveDis +=t;

            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.25;
            ymax =0.5;

            if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }

   }
   else if(left)
   {
       if(enmLoc.x>-1+unitWidth/2)
        {
            enmLoc.x -= t;
         }   moveDis +=t;

        xmin +=1/(float)frames;
        xmax +=1/(float)frames;
        ymin =0.75;
        ymax =1.0;

        if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }
   }

   else if(right)
   {
       if(enmLoc.x<1-unitWidth/2)
          {
              enmLoc.x += t;
           }   moveDis +=t;

            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;

            if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
              }
     }

  }else T1->Reset();
}
 else {moveDis =0;down=up=left=right=false; }
}


GridLoc Enemies::getEnemyLoc()
{
   GridLoc val;
   val.x = (int)(ceil((enmLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((enmLoc.y +(1-unitWidth))/unitWidth));

    return val;
}

// this creates a the list of valid points from matrix
linkList* Enemies::createNodeList(int **arr, int a, int b){

    linkList* validPts = new linkList();

    for (int i = 0; i < a; i++) {

		for (int j = 0; j < b; j++) {

			if (arr[i][j] != 0) {

                Node* tempNode = new Node(i, j);
				validPts->addNode(tempNode);
			}
		}
	}
	return validPts;
};

//accepts linkList of valid points from matrix and creates adjacency list; basically lists of edges
MLinkList* Enemies::createAdjList(linkList* valid) {

    Node* p = valid->root;
    MLinkList* master = new MLinkList();

    while (p!= nullptr){

        linkList* tempList = master->addLinkList(p);
        Node* tempNode = valid->root;

        while (tempNode!= nullptr){

            if (tempNode->a == p->a && ((tempNode->b== (p->b + 1)) || (tempNode->b== (p->b -1)))){ // checks if there is a point that is adjacent above or below
                tempList->addNode(tempNode);
            }

            else if (tempNode->b == p->b && ((tempNode->a== (p->a + 1)) || (tempNode->a== (p->a -1)))) // checks if there is a point that is adjacent to left or right
                tempList->addNode(tempNode);

            tempNode = tempNode->next;
        }

        p= p->next;

    }
	return master;
}

//initializes Dijkstras shortest path algorithm
Node* Enemies::shortestPath(linkList* valid, MLinkList* adjList, Player one){

    minHeap* sPath = new minHeap();
    Node* visited[valid->n];
    Node* unvisited[valid->n];


    //this section initializes the unvisited list
    Node* p = valid->root;
    while(p != nullptr){
            int i = 0;
            unvisited[i] = p;
            p= p->next;
    }

    //resets temp variable p, initializes the minHeap
    p = valid->root;
    while(p != nullptr){
        sPath->addHeapNode(p);
        p = p->next;
    }

    // this is the start point of the shortest path (enemies current location)
     int enmX = getEnemyLoc().x;
     int enmY = getEnemyLoc().y;

     Node* enemyNode = valid->lookup(enmX, enmY);

     // this is the destination point of the shortest path (players current location)
     int playX = one.getPlayerLoc().x;
     int playY = one.getPlayerLoc().y;

     Node* playNode = valid->lookup(playX, playY);

     //traverses AdjList until it finds the pointer that matches the data given from enmLoc
     linkList * l = adjList->head;
     while(( l->x != enmX) && (l->y != enmY) ){
        l = l->nextLL;
     }

     //once found we grab pointer of head of link list which is the vertex, initialize the source to proper dist and prev pointer
      Node * source = l->root;
      minHeapNode * temp = sPath->head;
      while (temp->vertex != source){
        temp = temp->next;
      }
      temp->distSrc = 0;
      temp->prev = nullptr;

      minHeapNode* start = temp;
      sPath->updateInfo(start, adjList, visited, unvisited, valid->n);
      return sPath->nextPos(enemyNode, playNode);
}
