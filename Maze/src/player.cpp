/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Player.h>

Timer *T = new Timer();                             // Set Timer for animation

Player::Player()
{
    //ctor
    playerDir = "down";
    arrowLoc.x = 0.2;
    arrowLoc.y =0.5;
    arrAngle =0;
    arrowStatus = false;
    livePlayer = true;
    moveState = true;
}

Player::~Player()
{
    //dtor
}

void Player::drawArrow()
{

if(arrowStatus){
   glColor3f(1.0,1.0,1.0);

     if(T->GetTicks()>10)
        {
            if(arrowLoc.x>-1+unitWidth/2 && arrowLoc.x<1-unitWidth/2)
                arrowLoc.x += t*arrXdir;
            else arrowStatus = false;
            if(arrowLoc.y>-1+unitWidth/2 && arrowLoc.y<1-unitWidth/2)
                arrowLoc.y += t*arrYdir;
            else arrowStatus = false;
           T->Reset();
        }
    glTranslatef(arrowLoc.x ,arrowLoc.y,0.0);
    glRotated(arrAngle,0,0,1);
    glBindTexture(GL_TEXTURE_2D,arrowTex);
    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);
     glEnd();
  }
}

//Function to shoot the arrow
void Player::shootArrow(){
    //Check if the player is alive
    if(livePlayer){
        //Check if arrow is still valid
        if(!arrowStatus){
            arrowStatus=true;

            //go left
            if(strcmp(playerDir, "left")==0){
                arrAngle = -90;
                arrXdir=-1;
                arrYdir=0;
                arrowLoc.x = plyLoc.x-unitWidth/2;
                arrowLoc.y = plyLoc.y;
            }

            //go right
            if(strcmp(playerDir, "right")==0){
                arrAngle =90;
                arrXdir=1;
                arrYdir=0;
                arrowLoc.x = plyLoc.x+unitWidth/2;
                arrowLoc.y = plyLoc.y;
            }

            //go up
            if(strcmp(playerDir, "up")==0){
                arrXdir=0;
                arrYdir=1;
                arrAngle =180;

                arrowLoc.y = plyLoc.y+unitWidth/2;
                arrowLoc.x = plyLoc.x;
            }

            //go down
            if(strcmp(playerDir, "down")==0){
                arrXdir=0;
                arrYdir=-1;
                arrAngle= 0;

                arrowLoc.y = plyLoc.y-unitWidth/2;
                arrowLoc.x = plyLoc.x;
            }
        }
    }
}

//Function for loading the image of the arrow
void Player::loadArrowImage(char* fileName)
{
    arrowTex = TextureLoader(fileName);
}

GridLoc Player::getArrowLoc()
{
   GridLoc val;

    val.x = (int)(ceil((arrowLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((arrowLoc.y +(1-unitWidth))/unitWidth));

   return val;
}


void Player::drawplayer(bool moveState, char* walking, char* firing)
{
    if(livePlayer){

        glColor3f(1.0,1.0,1.0);

        glTranslatef(plyLoc.x ,plyLoc.y,0.0);

        if(moveState == true){
            plyTex = TextureLoader(walking);
        }
        else{
            plyTex = TextureLoader(firing);
        }

        glBindTexture(GL_TEXTURE_2D,plyTex);
        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);


        glBegin(GL_QUADS);
            glTexCoord2f(xmin,ymin);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(xmax,ymin);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(xmax,ymax);
            glVertex3f(-1,-1,0.0f);

            glTexCoord2f(xmin,ymax);
            glVertex3f(1,-1,0.0f);
        glEnd();
    }
}


void Player::initPlayer(int gSize,char *FileName,int frames)
{
    gridSize = gSize;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove = 1;

    arrowAmount = 9;

    T->Start();
    plyLoc.x= -unitWidth;
    plyLoc.y= -unitWidth;

    t= unitWidth/stepsPerMove;

    plyTex = TextureLoader(FileName);
    livePlayer = true;
}


void Player::placePlayer(int x, int y)
{
    plyLoc.x =  converter(x,y).x;
    plyLoc.y =  converter(x,y).y;
}

void Player::movePlayer(char* dir, int frames){
    if(strcmp(dir, "left")==0){
        playerDir = "left";
        if(T->GetTicks()>10){
            if(plyLoc.x>-1+unitWidth/2){
                plyLoc.x -= t;
            }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin += 1/(float)frames;
            xmax += 1/(float)frames;
            ymin =0.75;
            ymax =1.0;
            T->Reset();
        }
    }

    else if(strcmp(dir, "right")==0){
        playerDir = "right";
        if(T->GetTicks()>1){
            if(plyLoc.x<1-unitWidth/2){
                plyLoc.x += t;
            }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;
            T->Reset();
        }
    }

    else if(strcmp(dir, "up")==0){
        playerDir = "up";
        if(T->GetTicks()>1){
            if(plyLoc.y< 1-unitWidth/2){
                plyLoc.y += t;
            }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;

            ymin =0.25;
            ymax =0.50;

            T->Reset();
        }
    }

   else if(strcmp(dir, "down")==0){
        playerDir = "down";
        if(T->GetTicks()>1){
            if(plyLoc.y > -1+unitWidth/2){
                plyLoc.y -= t;
            }
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.0;
            ymax =0.25;
            T->Reset();
        }
   }
}

void Player::movePlayerFace(char* dir, int frames){
    if(strcmp(dir, "left")==0){
        playerDir = "left";
        //if(T->GetTicks()>10){
            //if(plyLoc.x>-1+unitWidth/2){
            //    plyLoc.x -= t;
            //}
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin += 1/(float)frames;
            xmax += 1/(float)frames;
            ymin =0.75;
            ymax =1.0;
        //    T->Reset();
        //}
    }

    else if(strcmp(dir, "right")==0){
        playerDir = "right";
        //if(T->GetTicks()>1){
            //if(plyLoc.x<1-unitWidth/2){
            //    plyLoc.x += t;
            //}
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;
        //    T->Reset();
        //}
    }

    else if(strcmp(dir, "up")==0){
        playerDir = "up";
        //if(T->GetTicks()>1){
            //if(plyLoc.y< 1-unitWidth/2){
            //    plyLoc.y += t;
            //}
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;

            ymin =0.25;
            ymax =0.50;

        //    T->Reset();
        //}
    }

   else if(strcmp(dir, "down")==0){
        playerDir = "down";
        //if(T->GetTicks()>1){
            //if(plyLoc.y > -1+unitWidth/2){
            //    plyLoc.y -= t;
            //}
            if(xmax>=1){
                xmax =1/(float)frames;
                xmin =0;
            }
            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.0;
            ymax =0.25;
        //    T->Reset();
        //}
   }
}

GridLoc Player::getPlayerLoc()
{
   GridLoc val;

   val.x = (int)(ceil((plyLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((plyLoc.y +(1-unitWidth))/unitWidth));

   return val;

}

loc Player::converter(int x, int y)
{
  loc val;
        x+=1;
        y+=1;

       val.x = -1-unitWidth/2+(unitWidth)*x;
       val.y = -1-unitWidth/2+(unitWidth)*y;
       return val;
}
