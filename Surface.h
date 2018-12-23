#ifndef _SURFACE_H_
#define _SURFACE_H_
#include <stdio.h>

//#include "Patch.h"
#include "Window.h"
class Patch;


class Surface
{
private:


public:
    Surface();
//    ~Surface();
    
    Patch * left;  //sharing [0][3],[1][3],[2][3],[3][3]
    Patch * right; //sharing [0][0],[1][0],[2][0],[3][0]
 
    void regenerate();
    void draw(GLuint);
    void scale(float s);
    void translate(float x, float y, float z);
    void move(float x, float y, float z);
};


#endif /* _SURFACE_H_ */
