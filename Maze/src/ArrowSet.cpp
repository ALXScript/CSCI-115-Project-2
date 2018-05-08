#include <ArrowSet.h>
#include <iostream>

using namespace std;

arrowBundle::arrowBundle(){
    //constructor
}

arrowBundle::~arrowBundle(){
    //destructor
}

void arrowBundle::bundleInit(int grid, char* FileName){
    valid = true;
    gridSize = grid;
    unitWidth = (float)2.0/grid;
    bundleTex = TextureLoader(FileName);
}

//OpenGL Draws the Arrow Set
void arrowBundle::drawBundle()
{
    glColor3f(1.0,1.0,1.0);

    //glRotated(-spin,0,0,1);

    glBindTexture(GL_TEXTURE_2D,bundleTex);

    glPushMatrix();
    glTranslatef(bundleBrk.x,bundleBrk.y,0.0);
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
     glPopMatrix();
}

void arrowBundle::placeBundle(int x, int y){
    unitWidth = 2.0/gridSize;

    x+=1;
    y+=1;

    bundleBrk.x = -1-unitWidth/2+(unitWidth)*x;
    bundleBrk.y = -1-unitWidth/2+(unitWidth)*y;
}

GridLoc arrowBundle::GetBundleLoc()
{
   GridLoc val;

   val.x = (int)(ceil((bundleBrk.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((bundleBrk.y +(1-unitWidth))/unitWidth));

   return val;

}
