/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Maze.h>


Maze::Maze()
{

}

//gets the size of the maze to generate
Maze::Maze(int gSize)                                        // Constructor
{
    //ctor
    gridSize =gSize;

    unitWidth = (float)2/gridSize;
    liveChest = true;
    liveSetOfArrws=true;
}

//destructor
Maze::~Maze()
{
    //dtor
}

//loads the image of the chest
void Maze::loadChestImage(char* FileName)
{
    chestTex = TextureLoader(FileName);
}

//loads the background
void Maze::loadBackgroundImage(char* FileName)
{
   bakTex = TextureLoader(FileName);
}

//loads the arrows
void Maze::loadSetOfArrowsImage(char* FileName)
{
    ArrBnchTex =  TextureLoader(FileName);
}

//places the chest in a designated location
void Maze::placeChest(int x, int y)
{
   chestLoc.x =  converter(x,y).x;
   chestLoc.y =  converter(x,y).y;
}

//places the arrows in a designated location
void Maze::placeStArrws(int x, int y)
{
   setOfArrsLoc.x =  converter(x,y).x;
   setOfArrsLoc.y =  converter(x,y).y;
}


//grabs the current location of the chest (use for collision)
GridLoc Maze::GetChestLoc()
{
   GridLoc val;

   val.x = (int)(ceil((chestLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((chestLoc.y +(1-unitWidth))/unitWidth));

   return val;
}


//grabs the current location of the arrows (use for collision)
GridLoc Maze::GetStArrwsLoc()
{
    GridLoc val;

   val.x = (int)(ceil((setOfArrsLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((setOfArrsLoc.y +(1-unitWidth))/unitWidth));

   return val;
}

//grabs the size of the grid
int Maze::getGridSize()
{
  return gridSize;
}

//OpenGL Draws the background
void Maze::drawBackground()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,bakTex);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();
}

//OpenGL Draws the grid
void Maze::drawGrid()
{
   float a;
   glColor4f(1.0,1.0,1.0,0.2);
   glDisable(GL_TEXTURE_2D);
   glPointSize(1);
   glBegin(GL_LINES);

   for(int i =0;i<gridSize;i++)
    {
         a = -1+(float)unitWidth*i;
         glVertex3f(a,1,-0.4);
         glVertex3f(a,-1,-0.4);
         glVertex3f(-1,a,0.4);
         glVertex3f(1,a,0.4);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

//OpenGL Draws the arrows
void Maze::drawArrows()
{
    if(liveSetOfArrws)
    {

    glColor3f(1.0,1.0,1.0);

    glTranslatef(setOfArrsLoc.x,setOfArrsLoc.y,1.0);
    glRotated(-spin,0,0,1);

    glBindTexture(GL_TEXTURE_2D,ArrBnchTex);
    glScaled(1.0/(float)(gridSize),1.0/(float)(gridSize),1.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();
    }
}

//OpenGL Draws the chest
void Maze::drawChest()
{
    if(liveChest){
    glColor3f(1.0,1.0,1.0);

    glTranslatef(chestLoc.x,chestLoc.y,1.0);

    glBindTexture(GL_TEXTURE_2D,chestTex);
    glScaled(1.0/(float)(gridSize+5),1.0/(float)(gridSize+5),1.0);
    glRotated(spin,0,0,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

       glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
     glEnd();

     spin +=0.5;
    }
}

//don't know what this does yet
loc Maze::converter(int x, int y)
{
       loc val;

        x+=1;
        y+=1;
       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}
//<<<<<<< HEAD
//=======

void Maze::createAdjList(int **arr, int a, int b) {

	MLinkList* masterLL = new MLinkList();
	for (int i = 0; i < a; i++) {
		linkList * temp = masterLL->addLinkList(i);
		for (int j = 0; j < b; j++) {
			if (arr[i][j] != 0) {
				temp->addNode(j);
			}
		}
	}
}


//>>>>>>> 906fc632150c0700fcd8ff8486ca142f11d91dc8
