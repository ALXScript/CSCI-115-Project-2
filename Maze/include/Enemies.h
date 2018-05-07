/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef ENEMIES_H
#define ENEMIES_H

#include"CommonThings.h"
#include "Maze.h"
#include "player.h"
#include <time.h>
#include <thread>
#include <string>
#include <math.h>       /* ceil */

class Enemies
{
    public:
        Enemies();                          // constructor
        virtual ~Enemies();                 // DeConstructor
        void initEnm(int, int, char *);     // Initialize Enemy

        void placeEnemy(int,int);           // place enemy
        void drawEnemy();                   // Draw Enemy with Animation
        void moveEnemy(string);             // move Enemy left,right,up,down
        void animate();                     // Animate sprite
        GridLoc getEnemyLoc();              // Return Enemy current grid location


        int gridSize;                       // Grid size of the maze
        float unitWidth;                    // Unit width of the grid
        int stepsPerMove;                   // Step count for animation
        bool live;                          // Status of the Enemy


        linkList* createNodeList(int **arr, int a, int b);                      // creates possible path nodes
        MLinkList* createAdjList(linkList* valid);	                            //function that takes and creates matrix assuming that 0 means a wall (not sure where we would put this but its here temp
        Node* shortestPath(linkList* valid, MLinkList* AdjList, Player one);    // accepts enemy location and player location. Will calculate Shortest path between enemy and player and return Node* containing X,Y of next move


    protected:

    private:
         int frames;                        // number of frames generally
         float t;                           // steps for animation count
         GLuint enmTex;                     // Image Texture

         float xmax, xmin,ymax,ymin;        // Animated Texture mapping
         bool up,down,left,right;           // move direction
         float moveDis=0;                   // Moving distance for animation
         loc enmLoc;                        // location of the enemy
};

#endif // ENEMIES_H
