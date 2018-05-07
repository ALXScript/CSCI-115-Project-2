#ifndef ARROWSET_H_INCLUDED
#define ARROWSET_H_INCLUDED

#include "CommonThings.h"
#include <string.h>
#include <gl/gl.h>
#include <GL/glut.h>
#include <SOIL.h>
#include <iostream>
#include "Timer.h"
#include <math.h>

class arrowBundle{
    public:
        arrowBundle();
        virtual ~arrowBundle();
        arrowBundle(int);

        void drawBundle();
        void bundleInit(int, char*);
        void placeBundle(int, int);

        float unitWidth;
        float spin;
        int gridSize;

    private:
        loc bundleBrk;
        GLuint bundleTex;
        GridLoc GetBundleLoc;
};

#endif // ARROWSET_H_INCLUDED
