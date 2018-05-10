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

void Enemies::moveEnemy(Node** validPts, int sizearr, linkList* adjList,Player* one)
{
    if(getEnemyLoc().x == one->getPlayerLoc().x && getEnemyLoc().y == one->getPlayerLoc().y) return;
    Node* dest = shortestPath(validPts,sizearr,adjList,one); // shortest path is problem
    if(moveDis<=0){
        if(dest->a == getEnemyLoc().x && dest->b > getEnemyLoc().y){ up=true; down=left=right=false;}
        else if(dest->a == getEnemyLoc().x && dest->b < getEnemyLoc().y){down=true; up=left=right=false;}
        else if(dest->b == getEnemyLoc().y && dest->a < getEnemyLoc().x){left=true; down=up=right=false;}
        else if(dest->b == getEnemyLoc().y && dest->a > getEnemyLoc().x){right=true; down=left=up=false;}
        else {up=down=left=right=false;}
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

//initializes Dijkstras shortest path algorithm
Node* Enemies::shortestPath(Node** valid, int sizeArr, linkList* adjList, Player* one){

    minHeap* sPath = new minHeap();
    Node* visited[sizeArr];


    //this section initializes the unvisited list
    int c = 0;
    Node* p = valid[c];
    while(p != nullptr && c< sizeArr){
        sPath->addHeapNode(valid[c]);
        c++;
    }

    // this is the start point of the shortest path (enemies current location)
     int enmX = getEnemyLoc().x;
     int enmY = getEnemyLoc().y;

     Node* enemyNode = adjList->lookup(enmX, enmY);

     // this is the destination point of the shortest path (players current location)
     int playX = one->getPlayerLoc().x;
     int playY = one->getPlayerLoc().y;

     Node* playNode = adjList->lookup(playX, playY);


      Node * source = enemyNode;
      minHeapNode * temp = sPath->head;
      while (temp->vertex != source && temp!=nullptr){
        temp = temp->next;
      }
      temp->distSrc = 0;
      temp->prev = nullptr;

      minHeapNode* start = temp;
      int cn = 0;
      sPath->updateInfo(start, adjList, visited, sizeArr, source, cn);
      Node* nextP = sPath->nextPos(enemyNode, playNode);
      sPath->cleanArray(visited, sizeArr);
      delete sPath;


      return nextP;
}
