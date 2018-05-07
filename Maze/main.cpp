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

/* GLUT callback Handlers */

using namespace std;

///GLOBAL VARIABLES FOR INITIALIZATION
int mazeSize = 20;          //For setting the Maze Grid Size
int currentWallNumber = 0;  //For getting the max amount of walls in the txt file
int currentEnemyNumber = 0; //For getting the max amount of enemies in the txt file
char* imageBackground = "images/Danyu/bak.jpg";
char* imageVictory = "images/Ours/Victory.png";
char* imageChest = "images/Danyu/testchest.png";
char* imageArrowSet = "images/Danyu/arrwset.png";
char* imagePlayerMoving = "images/Danyu/p.png";
char* imagePlayerFire = "images/Ours/pFire.png";
char* imageArrow = "images/Danyu/arr.png";
char* imageWall = "images/Danyu/wall.png";
char* imageEnemy = "images/Danyu/e.png";

///VARIABLES FOR INITIALIZING THE CLASSES
Maze *M = new Maze(mazeSize);//Set Maze Grid Size
Player *P = new Player();   //Create Player
wall W[400];                //Wall with number of bricks
Enemies E[200];             //Create number of enemies
Timer *T0 = new Timer();    //Animation timer

///GLOBAL VARIABLES FOR GAME STATES
bool activeGame = false;    //For making the game menu (boolean states)
bool lvl1Complete = false;  //For checking Level 1 Completion

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
linkList* createNodeList(int arr[20][20], int a, int b){

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
}

//function that takes and creates matrix assuming that 0 means a wall (not sure where we would put this but its here temp
MLinkList* createAdjList(linkList* valid){

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
//Function for cout-ing the matrix (for debugging)
void showMatrix(){
    //cout the matrix for debugging
    cout << "Matrix: " << endl;
    //for(int i = 0; i < 20; i++){
    //    for(int j = 0; j < 20; j++){
    //        cout << "[" << i << "][" << j << "] = " << matrix[i][j] << endl;;
    //    }
    //    cout << endl;
    //}
    for(int i = 1; i < 3; i++){
        for(int j = 0; j < 10; j++){
            cout << "[" << i << "][" << j << "] = " << matrix[i][j] << endl;;
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

    //show the matrix
    showMatrix();
}

//Function for initializing the GL Window
void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    //read the file and set it in the array
    readFile();

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    M->loadBackgroundImage(imageBackground);           // Load maze background image
    M->loadChestImage(imageChest);              // load chest image

    //loop to get the chest location
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(matrix[i][j] == 5){
                M->placeChest(i, j);
                break;
            }
        }
    }

    //loading the chest image set
    M->loadSetOfArrowsImage(imageArrowSet);

    //loop to get the arrow set location
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(matrix[i][j] == 6)
                M->placeStArrws(i, j);
        }
    }

    //Loading the Player
    P->initPlayer(M->getGridSize(),imagePlayerMoving,6);   // initialize player pass grid size,image and number of frames
    P->loadArrowImage(imageArrow);                // Load arrow image

    //Loop to place the player
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(matrix[i][j] == 2){
                P->placePlayer(i, j);
                break;
            }
        }
    }

    //looks like here's where we're gonna read from text file where we place the walls
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(matrix[i][j] == 0){
                W[currentWallNumber].wallInit(M->getGridSize(),imageWall);// Load walls
                W[currentWallNumber].placeWall(i,j);                              // place each brick
                currentWallNumber++;
            }
        }
    }

    //same here, but for reading/placing the enemy locations
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 20; j++){
            if(matrix[i][j] == 3){
                E[currentEnemyNumber].initEnm(M->getGridSize(),4,imageEnemy); //Load enemy image
                E[currentEnemyNumber].placeEnemy(i, j);
                currentEnemyNumber++;
            }
        }
    }
    //linkList* validPts = createNodeList(matrix, 20, 20);
    //MLinkList* adjList = createAdjList(validPts);
}



void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

        //Only renders the background for a new game
        if(activeGame == false){
            glPushMatrix();
            M->drawBackground();
            glPopMatrix();
            glutSwapBuffers();
        }
        //'N' key pressed, new game made, display everything
        else
        {
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

            glPushMatrix();
                P->drawArrow();
            glPopMatrix();

            glPushMatrix();
            M->drawChest();
            glPopMatrix();

            glPushMatrix();
            M->drawArrows();
            glPopMatrix();

            glutSwapBuffers();
        }

}

//boolean function for checking collision detection
//NOTE: ATM JUST DETECTS WALLS NOTHING ELSE
bool checkPosition(char* direction){
/*
Legend:
0 = Wall
1 = Empty Space (walkable)
2 = Player Spawn
3 = Enemy Spawn
4 = Heart
5 = Chest
6 = Arrow Sets
7 = Player current position
*/
    //moving up
    if(strcmp(direction, "up")==0){
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;
        if(matrix[currentPlayerX][currentPlayerY+1] == 0){
            return false;
        }
        else if(matrix[currentPlayerX][currentPlayerY+1] == 5){
            M->loadBackgroundImage(imageVictory);
            activeGame = false;
            readFile();
            return true;
        }
        else{
            return true;
        }
    }

    //moving down
    else if(strcmp(direction, "down")==0){
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;
        if(matrix[currentPlayerX][currentPlayerY-1] == 0){
            return false;
        }
        else if(matrix[currentPlayerX][currentPlayerY-1] == 5){
            M->loadBackgroundImage(imageVictory);
            activeGame = false;
            readFile();
            return true;
        }
        else{
            return true;
        }
    }

    //moving left
    else if(strcmp(direction, "left")==0){
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;
        if(matrix[currentPlayerX-1][currentPlayerY] == 0){
            return false;
        }
        else if(matrix[currentPlayerX-1][currentPlayerY] == 5){
            M->loadBackgroundImage(imageVictory);
            activeGame = false;
            readFile();
            return true;
        }
        else{
            return true;
        }
    }

    //moving right
    else if(strcmp(direction, "right")==0){
        currentPlayerX = P->getPlayerLoc().x;
        currentPlayerY = P->getPlayerLoc().y;
        if(matrix[currentPlayerX+1][currentPlayerY] == 0){
            return false;
        }
        else if(matrix[currentPlayerX+1][currentPlayerY] == 5){
            M->loadBackgroundImage(imageVictory);
            activeGame = false;
            readFile();
            return true;
        }
        else{
            return true;
        }
    }
}

//Function for checking Arrow-Enemy collision
int findEnemy(int arrowX, int arrowY){
    for(int i = 0; i < currentEnemyNumber; i++){
        if(E[i].getEnemyLoc().x == arrowX && E[i].getEnemyLoc().y == arrowY){
            return i;
        }
    }
    return -1;
}

//char convertDirection(char* direction){
//    if(direction == "up"){
//        return 'u';
//    }
//    else if(direction == "down"){
//        return 'd';
//    }
//    else if(direction == "left"){
//        return 'l';
//    }
//    else if(direction == "right"){
//        return 'r';
//    }
//}

//
int intConvertDirection(char* direction){
    if(direction == "up"){
        return 1;
    }
    else if(direction == "down"){
        return 2;
    }
    else if(direction == "left"){
        return 3;
    }
    else if(direction == "right"){
        return 4;
    }
    else{
        return -1;
    }
}



//Function for checking Arrow Position
void checkArrow(){
    char* direction = P->playerDir;

    int intDirection = intConvertDirection(direction);

    switch(intDirection){
    case 1:
        currentArrowX = P->getArrowLoc().x;
        currentArrowY = P->getArrowLoc().y;

        //loop through the matrix
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 20; j++){
                //if the next slot belongs to an enemy
                if(matrix[currentArrowX][currentArrowY+1] == 3){
                    //find out which enemy it is
                    int foundEnemy = findEnemy(currentArrowX, currentArrowY+1);
                    //case enemy found
                    if(foundEnemy != -1){
                        E[foundEnemy].live = false;
                    };
                }
            }
        }

    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case -1:
        break;
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

//Equivalent to Unity Update Function
 void idle(void){
    //check the state of the player
    if(P->livePlayer == false){
        //end the game
    }

    //check if level 1 is complete
    if(lvl1Complete == true){
        //end the game
    }

    //cout << "Player x: " << P->getPlayerLoc().x << "\tPlayer y: " << P->getPlayerLoc().y << endl;

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
        //space = shoot or walk
        case ' ':
            //if in move state, space = move
            if(P->moveState == true){
                //run collision detection
                if(checkPosition(P->playerDir) == false){
                    P->placePlayer(currentPlayerX, currentPlayerY);
                }
                else{
                    matrix[currentPlayerX][currentPlayerY] = 1;
                    P->movePlayer(P->playerDir,P->frames);
                    matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 7;
                    cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
                    showMatrix();
                }
            }
            //if in shoot state, space = shoot
            else{
                P->shootArrow();
            }
        break;
        //n = new game/start game
        case 'n':
            activeGame = true;
            break;
        //z = change state
        case 'z':
            P->moveState = !(P->moveState);
            break;
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
    switch(key)
    {
    case GLUT_KEY_UP:
        P->movePlayerFace("up", P->frames);
        //basic collision detection
        // if(checkPosition('u') == false){
        //    P->placePlayer(currentPlayerX, currentPlayerY);
        // }
        // else{
        //    matrix[currentPlayerX][currentPlayerY] = 1;
        //    P->movePlayer("up",P->frames);
        //    matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 7;
        //    cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
        //    showMatrix();
            ///SHORTEST PATH FOR ENEMIES HERE
            //E[0].moveEnemy("up");
            //E[1].moveEnemy("up");
            //E[2].moveEnemy("up");
        // }

    break;

    case GLUT_KEY_DOWN:
        P->movePlayerFace("down", P->frames);
        //if(checkPosition('d') == false){
        //    P->placePlayer(currentPlayerX, currentPlayerY);
        //}
        //else{
            //matrix[currentPlayerX][currentPlayerY] = 1;
            //P->movePlayer("down",P->frames);
            //matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 7;
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            //showMatrix();
            ///SHORTEST PATH FOR ENEMIES HERE
            //E[0].moveEnemy("down");
            //E[1].moveEnemy("down");
            //E[2].moveEnemy("down");
        //}
    break;

    case GLUT_KEY_LEFT:
        P->movePlayerFace("left", P->frames);
        //if(checkPosition('l') == false){
            //P->placePlayer(currentPlayerX, currentPlayerY);
        //}
        //else{
            //matrix[currentPlayerX][currentPlayerY] = 1;
            //P->movePlayer("left",P->frames);
            //matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 7;
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            //showMatrix();
            ///SHORTEST PATH FOR ENEMIES HERE
            //E[0].moveEnemy("left");
            //E[1].moveEnemy("left");
            //E[2].moveEnemy("left");
        //}
    break;

    case GLUT_KEY_RIGHT:
        P->movePlayerFace("right", P->frames);
        //if(checkPosition('r') == false){
            //P->placePlayer(currentPlayerX, currentPlayerY);
        //}
        //else{
            //matrix[currentPlayerX][currentPlayerY] = 1;
            //P->movePlayer("right",P->frames);
            //matrix[P->getPlayerLoc().x][P->getPlayerLoc().y] = 7;
            //cout<< P->getPlayerLoc().x<< "    "<<P->getPlayerLoc().y<<endl;
            //showMatrix();
            ///SHORTEST PATH FOR ENEMIES HERE
            //E[0].moveEnemy("right");
            //E[1].moveEnemy("right");
            //E[2].moveEnemy("right");
        //}
    break;

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
