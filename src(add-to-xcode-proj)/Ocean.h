#ifndef _OCEAN_H_
#define _OCEAN_H_
#include <stdio.h>

#include "Surface.h"
#include "Window.h"
class Surface;


class Ocean
{
private:

public:
    Ocean();
    
    Surface * top;
    Surface * bottom;
 
    void align();
    void draw(GLuint);
    void scale(float s);
    void regenerate();
    void translate(float x, float y, float z);
    glm::vec3 getCenter();
};


#endif /* _OCEAN_H_ */
