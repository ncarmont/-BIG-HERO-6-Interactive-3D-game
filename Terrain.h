#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "Node.h"

class Terrain: public Node
{
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> ordered;
    std::vector<glm::vec3> normals;
    
    const int N = 5;
    const int TOTAL = pow(2, N) + 1;
    unsigned int seed;
    GLint shader;
    
public:
    Terrain(GLint shaderProgram);
    ~Terrain();
    
    void setData();
    
    void diamond_step(int x, int y, int step, int magnitude);
    void square_step(int x, int y, int step, int magnitude);
    //    std::vector <glm::vec3> returnVertices(int num, glm::vec3 translation, float scale);
    
    void draw(glm::mat4 C);
    void update();
    
    GLuint VBO, VAO, VBO2;
    
};

#endif
