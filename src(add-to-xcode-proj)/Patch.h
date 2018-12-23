#ifndef _PATCH_H_
#define _PATCH_H_
#include <stdio.h>
#include "Window.h"



class Patch
{
private:

    std::vector<glm::vec3> cp_v; //control points/vertices
    
    std::vector<int> cp_i; //indices

    GLuint VBO, VAO, EBO, VBON;

public:
    Patch();
    Patch(glm::vec3[4][4]);
    ~Patch();
    
    std::vector<glm::vec3> cp_n; //normals
    
    int segments;
    
    glm::vec3 patch[4][4];
    
    void regenerate_left();
    void regenerate_right();
    void setup();
    void populateIndices();
    void populateVertNorms();
    void rebindBuffers();
    void reset();
    
    glm::mat4 toWorld;

    void draw(GLuint);
    glm::vec3 Bez(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    glm::vec3 BezD(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    glm::vec3 calcPoint(float u, float v);
    glm::vec3 calcNormal(float u, float v);
    
    void scale(float s);
    void translate(float x, float y, float z);
    void move(float x, float y, float z);
    
};


#endif /* _PATCH_H_ */
