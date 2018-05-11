/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>
#include "ArrowSet.h"
//#include <ArrowSet.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>

//for reading the files
#include <sstream>
#include <fstream>

//for giving a small amount of break to read any important info
#include <windows.h>

/* GLUT callback Handlers */

using namespace std;

///GLOBAL VARIABLES FOR INITIALIZATION
int mazeSize = 20;          //For setting the Maze Grid Size
int currentWallNumber = 0;  //For getting the max amount of walls in the txt file
int currentEnemyNumber = 0; //For getting the max amount of enemies in the txt file
int currentBundleNumber = 0; //For getting the max amount of arrow bundles in the txt file
char* imageBackground = "images/Ours/GrassSeamless.png";
char* imageMain = "images/Ours/MainScreenP.png";
char* imageVictory = "images/Ours/Victory.png";
char* imageLose = "images/Ours/Loss.png";
char* imageChest = "images/Ours/testchest.png";
char* imageArrowSet = "images/Ours/arrwset.png";
char* imagePlayerMoving = "images/Danyu/p.png";
char* imagePlayerFire = "images/Ours/pFire.png";
char* imageArrow = "images/Ours/arr.png";
char* imageWall = "images/Ours/BrickSeamless.png";
char* imageEnemy = "images/Danyu/e.png";

///VARIABLES FOR INITIALIZING THE CLASSES
Maze *M = new Maze(mazeSize);//Set Maze Grid Size
Player *P = new Player();   //Create Player
wall W[400];                //Wall with number of bricks
Enemies E[200];             //Create number of enemies
Timer *T0 = new Timer();    //Animation timer
arrowBundle A[10];
linkList* adjList;
Node** validPts;
int sizeValPts;

///GLOBAL VARIABLES FOR GAME STATES
bool activeGame = false;    //For making the game menu (boolean states)
bool mainMenu = true;
bool lvl1Complete = false;  //For checking Level 1 Completion
bool allEnemiesDead = false;//For checking if all the enemies have been killed
bool noInputAllowed = false;
bool justN = true;
bool firstRun = true;
int enemiesKilled = 0;
int sleepTime = 2500;

///GLOBAL VARIABLES FOR THE VIEWPORT/DISPLAY WINDOWS
float wWidth, wHeight;      // display window width and Height
float xPos,yPos;            // Viewpoar mapping

///GLOBAL VARIABLES FOR READING THE MATRIX FROM A FILE
string lineA;                                   //temp string for reading the file
int matrix[20][20] = {{0}};                     //20x20 matrix for testing and fill all of it with 0
string filename = "txtFiles/maze20x20.txt";     //name of the .txt file that has the matrix in it
ifstream fileIN;                                //for reading the file

///GLOBAL VARIABLES FOR COLLISION DETECTION
int currentPlayerX; //For holding the current player's X position
int currentPlayerY; //For holding the current player's Y position
int currentArrowX;  //For holding the current arrow's X position
int currentArrowY;  //For holding the current arrow's Y position

///MAIN DISPLAY: THIS RUNS IN A LOOP
void display(void); // Main Display : this runs in a loop

///Resizing case on the window
void resize(int width, int height)
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}
// creates possible path nodes
Node** createNodeList(int arr[20][20], int a, int b, int &sizesArr){ //working perfect

    Node **validPts = new Node*[400];
    for (int i = 0; i < a; i++) {

		for (int j = 0; j < b; j++) {

			if (arr[i][j] != 1) {

                Node* tempNode = new Node(i, j);
				validPts[sizesArr] = tempNode;

				sizesArr++;

			}
		}
	}

	return validPts;
}

//function that takes and creates matrix assuming that 0 means a wall (not sure where we would put this but its here temp
linkList* createAdjList(Node* valid[400], int sizeArr){

    linkList* master = new linkList();
    //cout<<sizeArr<<endl;

    for (int i = 0; i < sizeArr; i++){
        Node* tempNode = valid[i]; //causing problems
        //cout<<tempNode->a<<" ," <<tempNode->b <<endl;

        master->addNode(valid[i]);
        for (int j = 0; j < sizeArr; j++){
            Node* p = valid[j];

            //cout<<"CURRENTLY CHECKING: "<<p->a<< " ,"<< p->b<<endl;

            if (valid[i]->a == valid[j]->a && ((valid[i]->b == (valid[j]->b + 1)) || (valid[i]->b == (valid[j]->b -1)))){ // checks if there is a point that is adjacent above or below

            //cout<<"checked first condition of nested for loop and passed"<< endl;

            master->updateNeighbors(valid[i], valid[j]);

            //cout<<"UPDATED NEIGHBOR OF "<<valid[i]->a<< " ,"<< valid[i]->b<< " WITH: "<< valid[j]->a<< ", "<< valid[j]->b<< endl;
            }

            else if (valid[i]->b == valid[j]->b && ((valid[i]->a== (valid[j]->a + 1)) || (valid[i]->a== (valid[j]->a -1)))){ // checks if there is a point that is adjacent to left or right

            //cout<<"checked second condition of nested for loop and passed"<< endl;

            master->updateNeighbors(valid[i], valid[j]);

            //cout<<"UPDATED NEIGHBOR OF "<<valid[i]->a<< " ,"<< valid[i]->b<< " WITH: "<< valid[j]->a<< ", "<< valid[j]->b<< endl;
            }
        }

    }

	return master;
}

//Function for cout-ing the matrix (for debugging)
void showMatrix(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

//Function for reading the .txt file
void readFile(){
    //open the file
    fileIN.open(filename);

    //error check
    if(fileIN.fail()){
        cerr << "* File not found, cannot be opened!";
        exit(1);
    }

    //read the data file and put in in the matrix
    while(fileIN.good()){
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 20; j++){
                fileIN >> matrix[i][j];
            }
        }
    }

    fileIN.close();

    //show the matrix
    showMatrix();
}

void resetGlobals(){
 activeGame = false;    //For making the game menu (boolean states)
 mainMenu = true;
 lvl1Complete = false;  //For checking Level 1 Completion
 allEnemiesDead = false;//For checking if all the enemies have been killed
 noInputAllowed = true;
 justN = true;
 firstRun = false;
 enemiesKilled = 0;
 currentPlayerX = 0; //For holding the current player's X position
 currentPlayerY = 0; //For holding the current player's Y position
 currentArrowX = 0;  //For holding the current arrow's X position
 currentArrowY = 0;  //For holding the current arrow's Y position

 P->moveState = true;
 P->movePlayerFace("down", P->frames);
}

//Function for initializing the GL Window
void init()
{
    if(firstRun == true){

        glEnable(GL_COLOR_MATERIAL);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClearColor(0.0,255.0,0.0,0.0);
        gluOrtho2D(0, wWidth, 0, wHeight);
    }

    cout << "\nBefore initReadFile\n";
    //read the file and set it in the array
    readFile();

    cout << "\n\nBefore Timer Start\n\n";
    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cout << "\n\nBefore Load background and chest\n\n";
    //Load all images
    M->loadBackgroundImage(imageMain);           // Load maze background image
    M->loadChestImage(imageChest);              // load chest image

    cout << "\n\nBefore Init player & Load arrow image\n\n";
    //Loading the Player
    //cout << endl << endl << "BEFORE PLAYER INIT" << endl << endl;
    P->initPlayer(M->getGridSize(),imagePlayerMoving,6);   // initialize player pass grid size,image and number of frames
    P->loadArrowImage(imageArrow);                // Load arrow image

    //cout << endl << endl << "AFTER PLAYER INIT" << endl << endl;

    cout << "\n\nBefore loop to place matrix\n\n";
    if(firstRun == true){
        //loop to place a lot of things
        for(int i = 0; i < 20; i++){
            //cout << "\n\nPlacing everything\n\n";
            for(int j = 0; j < 20; j++){
                //if it's a wall
                if(matrix[i][j] == 1){
                    W[currentWallNumber].wallInit(M->getGridSize(),imageWall);// Load walls
                    W[currentWallNumber].placeWall(i,j);                              // place each brick
                    currentWallNumber++;
                }

                //If it's the enemy
                else if(matrix[i][j] == 2){
                    E[currentEnemyNumber].initEnm(M->getGridSize(),4,imageEnemy); //Load enemy image
                    E[currentEnemyNumber].placeEnemy(i,j);
                    currentEnemyNumber++;
                }

                //If it's the player
                else if(matrix[i][j] == 3){
                    P->placePlayer(i,j);
                }

                //if it's a chest
                else if(matrix[i][j] == 4){
                    M->placeChest(i, j);
                }

                else if(matrix[i][j] == 5){

                    A[currentBundleNumber].bundleInit(20, imageArrowSet);
                    A[currentBundleNumber].placeBundle(i,j);
                    currentBundleNumber++;
                }
            }
        }
        cout << "\n\nBefore validpts\n\n";
        validPts = createNodeList(matrix, 20, 20, sizeValPts);

        cout << "\n\nBefore AdjList\n\n";
        adjList = createAdjList(validPts, sizeValPts);

        firstRun = false;
    }
    else{
        P->livePlayer = true;
        currentWallNumber = 0;
        currentEnemyNumber = 0;
        currentBundleNumber = 0;
         //loop to place a lot of things
        for(int i = 0; i < 20; i++){
            //cout << "\n\nPlacing everything\n\n";
            for(int j = 0; j < 20; j++){
                //if it's a wall
                if(matrix[i][j] == 1){
                    W[currentWallNumber].placeWall(i,j);                              // place each brick
                    currentWallNumber++;
                }

                //If it's the enemy
                else if(matrix[i][j] == 2){
                    E[currentEnemyNumber].live = true;
                    E[currentEnemyNumber].placeEnemy(i,j);
                    currentEnemyNumber++;
                }

                //If it's the player
                else if(matrix[i][j] == 3){
                    P->placePlayer(i,j);
                }

                //if it's a chest
                else if(matrix[i][j] == 4){
                    M->placeChest(i, j);
                }

                else if(matrix[i][j] == 5){
                    A[currentBundleNumber].valid = true;
                    A[currentBundleNumber].placeBundle(i,j);
                    currentBundleNumber++;
                }
            }
        }
        cout << "\nBefore Reseting Globals\n";
        resetGlobals();
    }
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        //Only renders the background for a new game
        if(activeGame == false && mainMenu == true && P->livePlayer == true && lvl1Complete == false){
            cout << "\n\nMAIN MENU\n\n";
            M->loadBackgroundImage(imageMain);
            glPushMatrix();
            //cout << "\n\nDRAWING BACKGROUND\n\n";
            M->drawBackground();
            glPopMatrix();
            glutSwapBuffers();
        }

        //if you won the game, display only victory message
        else if(activeGame == false && lvl1Complete == true && P->livePlayer == true){
            cout << "\n\nYOU WON\n\n";
            noInputAllowed = true;
            M->loadBackgroundImage(imageVictory);
            glPushMatrix();
            M->drawBackground();
            glPopMatrix();
            glutSwapBuffers();

            Sleep(sleepTime);

            init();
        }

        //If the player died display Game Over
        else if(activeGame == false && P->livePlayer == false){
            cout << "\n\nYOU DIED\n\n";
            noInputAllowed = true;
            M->loadBackgroundImage(imageLose);
            glPushMatrix();
            M->drawBackground();
            glPopMatrix();
            glutSwapBuffers();

            Sleep(sleepTime);

            init();
        }

        //'N' key pressed, new game made, display everything
        else if(activeGame == true)
        {
            M->loadBackgroundImage(imageBackground);
            glPushMatrix();
            M->drawBackground();
            glPopMatrix();

            //SET RESTRICTIONS FOR SPAWNING WALLS
            for(int i=0; i<currentWallNumber;i++)
            {
                W[i].drawWall();
            }

            glPushMatrix();
                M->drawGrid();
            glPopMatrix();

            glPushMatrix();
                P->drawplayer(P->moveState, imagePlayerMoving, imagePlayerFire);
            glPopMatrix();

            //SET RESTRICTIONS FOR SPAWNING ENEMIES
            for(int i=0; i<currentEnemyNumber;i++)
            {
                //if enemy is alive, display it
                if(E[i].live == true)
                {
                    E[i].drawEnemy();
                }
            }

            //draws the arrow being fired
            glPushMatrix();
                P->drawArrow();
            glPopMatrix();

            //draws the chest
            glPushMatrix();
                M->drawChest();
            glPopMatrix();

            //draws the arrow sets
            //   glPushMatrix();
            //   M->drawArrows();
            for(int i = 0; i < currentBundleNumber; i++){
                 if(A[i].valid == true){
                     A[i].drawBundle();
                 }
            }
            //glPopMatrix();

            glutSwapBuffers();
        }

}

//function for collecting the arrow
void arrowCollected(int playerX, int playerY){
    //loop to find out which arrow set it is
    for(int i = 0; i < currentBundleNumber; i++){
        if(A[i].GetBundleLoc().x == playerX && A[i].GetBundleLoc().y == playerY){
            A[i].valid = false;
            P->arrowAmount = P->arrowAmount + 1;
        }
    }
}

void enemyCollision(Enemies &enemy, Player* one){


    if(enemy.getEnemyLoc().x == one->getPlayerLoc().x && enemy.getEnemyLoc().y == one->getPlayerLoc().y && activeGame == true){
        one->livePlayer == false;

    }

}

//Boolean function for checking collision detection
bool collisionDetection(char* direction){
/*
0 = Empty Space (Walkable)
1 = Wall (full)
2 = Enemy
3 = Player
4 = Chest
5 = Bags of Arrows
*/
    //moving up
    if(strcmp(direction, "up")==0){
        //save current player location
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;

        //IF WALL (1)
        if(matrix[currentPlayerX][currentPlayerY+1] == 1){
            return false;
        }

        //If Arrow Set (5)
        else if(matrix[currentPlayerX][currentPlayerY+1] == 5){
            //add arrows to inventory
            arrowCollected(currentPlayerX, currentPlayerY+1);
            return true;
        }

        //IF CHEST (4)
        else if(matrix[currentPlayerX][currentPlayerY+1] == 4){
            lvl1Complete = true;
            return true;
        }

        //IF Enemy (2)
        else if(matrix[currentPlayerX][currentPlayerY+1] == 2){
            //kill the player
            P->livePlayer = false;
            return true;
        }

        //ELSE EMPTY SPACE
        else{
            return true;
        }
    }

    //moving down
    else if(strcmp(direction, "down")==0){
        //save current player location
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;

        //IF WALL (1)
        if(matrix[currentPlayerX][currentPlayerY-1] == 1){
            return false;
        }

        //If Arrow Set (5)
        else if(matrix[currentPlayerX][currentPlayerY-1] == 5){
            //add arrows to inventory
            arrowCollected(currentPlayerX, currentPlayerY-1);
            return true;
        }

        //IF CHEST (4)
        else if(matrix[currentPlayerX][currentPlayerY-1] == 4){
            lvl1Complete = true;
            return true;
        }

        //IF Enemy (2)
        else if(matrix[currentPlayerX][currentPlayerY-1] == 2){
            //kill the player
            P->livePlayer = false;
            return true;
        }

        //ELSE EMPTY SPACE
        else{
            return true;
        }
    }

    //moving left
    else if(strcmp(direction, "left")==0){
        //save current player location
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;

        //IF WALL (1)
        if(matrix[currentPlayerX-1][currentPlayerY] == 1){
            return false;
        }

        //If Arrow Set (5)
        else if(matrix[currentPlayerX-1][currentPlayerY] == 5){
            //add arrows to inventory
            arrowCollected(currentPlayerX-1, currentPlayerY);
            return true;
        }

        //IF CHEST (4)
        else if(matrix[currentPlayerX-1][currentPlayerY] == 4){
            lvl1Complete = true;
            return true;
        }

        //IF Enemy (2)
        else if(matrix[currentPlayerX-1][currentPlayerY] == 2){
            //kill the player
            P->livePlayer = false;
            return true;
        }

        //ELSE EMPTY SPACE
        else{
            return true;
        }
    }

    //moving right
    else if(strcmp(direction, "right")==0){
        //save current player location
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;

        //IF WALL (1)
        if(matrix[currentPlayerX+1][currentPlayerY] == 1){
            return false;
        }

        //If Arrow Set (5)
        else if(matrix[currentPlayerX+1][currentPlayerY] == 5){
            //add arrows to inventory
            arrowCollected(currentPlayerX+1, currentPlayerY);
            return true;
        }

        //IF CHEST (4)
        else if(matrix[currentPlayerX+1][currentPlayerY] == 4){
            lvl1Complete = true;
            return true;
        }

        //IF Enemy (2)
        else if(matrix[currentPlayerX+1][currentPlayerY] == 2){
            //kill the player
            P->livePlayer = false;
            return true;
        }

        //ELSE EMPTY SPACE
        else{
            return true;
        }
    }
}

//Function for checking Arrow-Enemy collision
void findEnemy(int arrowX, int arrowY){
    for(int i = 0; i < currentEnemyNumber; i++){
        if(E[i].getEnemyLoc().x == arrowX && E[i].getEnemyLoc().y == arrowY && E[i].live == true){
            cout<< endl << endl<< "Enemy: "<< i << " has been killed"<< endl;
            E[i].live = false;
            if (E[i].prevValMatrix!=2)matrix[arrowX][arrowY] = E[i].prevValMatrix;
            else matrix[arrowX][arrowY] = 0;
            enemiesKilled++;
        }
    }
}


//Function for checking Arrow Position
bool checkArrow(char* direction){
    //shooting up
    if(strcmp(direction, "up")==0){

        currentArrowX = P->getPlayerLoc().x;
        currentArrowY = P->getPlayerLoc().y;

        //determine whether theres a wall or an enemy
        for(int i = 1; i < 20; i++){
            //enemy spotted, able to fire
            if(matrix[currentArrowX][currentArrowY+i] == 2){
                findEnemy(currentArrowX, currentArrowY+i);
                return true;
            }
            else if(matrix[currentArrowX][currentArrowY+i] == 1){
                return false;
            }
        }
    }

    //shooting down
    else if(strcmp(direction, "down")==0){

        currentArrowX = P->getPlayerLoc().x;
        currentArrowY = P->getPlayerLoc().y;

        //determine whether theres a wall or an enemy
        for(int i = 1; i < 20; i++){
            //enemy spotted, able to fire
            if(matrix[currentArrowX][currentArrowY-i] == 2){
                findEnemy(currentArrowX, currentArrowY-i);
                return true;
            }
            //wall spotted, no fire
            else if(matrix[currentArrowX][currentArrowY-i] == 1){
                return false;
            }
        }
    }

    //shooting left
    else if(strcmp(direction, "left")==0){

        currentArrowX = P->getPlayerLoc().x;
        currentArrowY = P->getPlayerLoc().y;

        //determine whether theres a wall or an enemy
        for(int i = 1; i < 20; i++){
            //enemy spotted, able to fire
            if(matrix[currentArrowX-i][currentArrowY] == 2){
                findEnemy(currentArrowX-i, currentArrowY);
                return true;
            }
            else if(matrix[currentArrowX-i][currentArrowY] == 1){
                return false;
            }
        }
    }

    //shooting right
    else if(strcmp(direction, "right")==0){

        currentArrowX = P->getPlayerLoc().x;
        currentArrowY = P->getPlayerLoc().y;

        //determine whether theres a wall or an enemy
        for(int i = 1; i < 20; i++){
            //enemy spotted, able to fire
            if(matrix[currentArrowX+i][currentArrowY] == 2){
                findEnemy(currentArrowX+i, currentArrowY);
                return true;
            }
            else if(matrix[currentArrowX+i][currentArrowY] == 1){
                return false;
            }
        }
    }
}

//Function for getting the OpenGL Position?
 void GetOGLPos(int x, int y){
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

//Function that moves the player
void moveThePlayer(){
    //if there is an invalid spot, do not move the player
    if(collisionDetection(P->playerDir) == false){
        P->placePlayer(currentPlayerX, currentPlayerY);
    }
    //else move the player and the enemies
    else{
        //change ability to take input to false
        noInputAllowed = true;

        //moves the player
        matrix[currentPlayerX][currentPlayerY] = 0;
        P->movePlayer(P->playerDir,P->frames);
        matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 3;
        cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;


        //re-enable ability to take inputs
        noInputAllowed = false;
        //showMatrix();
    }
}

//Equivalent to Unity Update Function
 void idle(void){
    //check if player is dead
    if(P->livePlayer == false){
        //game over
        activeGame = false;
        mainMenu = true;
    }

    //check if level 1 is complete
    if(lvl1Complete == true){
        //end the game
        activeGame = false;
        mainMenu = true;
    }


    if(enemiesKilled == currentEnemyNumber){
        //end the game
        activeGame = false;
        lvl1Complete = true;
        mainMenu = true;
    }

    //cout << "Player x: " << P->getPlayerLoc().x << "\tPlayer y: " << P->getPlayerLoc().y << endl;
    //cout << "Arrows: " << P->arrowAmount << endl;

    cout << "ActiveGame: " << activeGame << "   mainMenu: " << mainMenu << "  lvl1Com: " << lvl1Complete << "  All Enemies: " << allEnemiesDead << "  noInputAllowed: " << noInputAllowed << "  JustN: " << justN << "  EnKill: " << enemiesKilled << "  TotEnem: " << currentEnemyNumber << endl;
    //cout << "\n" << matrix[6][4] << endl;
    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y){

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;


      case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN){

              GetOGLPos(x,y);
            }
            break;
    }
     glutPostRedisplay();
};

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        if(noInputAllowed == true && justN == true){
            //n = new game/start game
            case 'n':
                activeGame = true;
                mainMenu = false;
                justN = false;
                noInputAllowed = false;
                break;
        }
        else if(noInputAllowed == false){
            //space = shoot or walk
            case ' ':
                //if in move state, space = move
                if(P->moveState == true ){
                    //loop is necessary for updating current locations of enemies
                    for (int i=0; i<currentEnemyNumber;i++){
                        if (E[i].live){
                            int enmA = E[i].getEnemyLoc().x;
                            int enmB = E[i].getEnemyLoc().y;
                            matrix[enmA][enmB] = 2;

                            //checks if enemy and player occupy same spot, if so then it kills player
                            enemyCollision(E[i], P);
                            idle();
                            if (activeGame == false) break;
                        }
                    }

                    //move the player
                    moveThePlayer();

                    for (int i=0; i< currentEnemyNumber;i++){
                        if (E[i].shortestPath(validPts,sizeValPts,adjList,P)->a == P->getPlayerLoc().x && E[i].shortestPath(validPts,sizeValPts,adjList,P)->b == P->getPlayerLoc().y ){
                            P->livePlayer == false;
                            idle();
                        }

                    }
                    idle();
                    if (activeGame==false) break;

                    if (activeGame){
                        for (int i = 0; i < currentEnemyNumber; i++){

                            if(E[i].live && P->livePlayer == true){
                                int enmA = E[i].getEnemyLoc().x;
                                int enmB = E[i].getEnemyLoc().y;
                                matrix[enmA][enmB]= E[i].prevValMatrix;
                                E[i].moveEnemy(validPts, sizeValPts, adjList, P, matrix, idle, activeGame);
                            }
                        }
                    }
                }
                //if in shoot state, space = shoot
                else{
                    if(P->arrowAmount > 0){
                        if(checkArrow(P->playerDir) == true){
                            noInputAllowed = true;
                            P->shootArrow();
                            P->arrowAmount = P->arrowAmount - 1;
                            noInputAllowed = false;
                            //showMatrix();
                        }
                    }
                }
                break;

            //z = change state
            case 'z':
                //Loop is necessary for updating matrix with new locations
                for (int i=0; i<currentEnemyNumber;i++){
                    if (E[i].live){
                        int enmA = E[i].getEnemyLoc().x;
                        int enmB = E[i].getEnemyLoc().y;
                        matrix[enmA][enmB] = 2;
                        enemyCollision(E[i], P);
                    }
                }
                P->moveState = !(P->moveState);
                break;
        }

        //esc key to exit
        case 27 :
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}

//Function for key inputs
void Specialkeys(int key, int x, int y)
{
    if(noInputAllowed == false){
        switch(key)
        {
        case GLUT_KEY_UP:
            P->movePlayerFace("up", P->frames);
        break;

        case GLUT_KEY_DOWN:
            P->movePlayerFace("down", P->frames);
        break;

        case GLUT_KEY_LEFT:
            P->movePlayerFace("left", P->frames);
        break;

        case GLUT_KEY_RIGHT:
            P->movePlayerFace("right", P->frames);
        break;
        }
    }

  glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
