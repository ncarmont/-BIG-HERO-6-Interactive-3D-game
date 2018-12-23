#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
//#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>



class BoundingSphere
{
private:

float radius;
glm::vec3 centerPoint;
glm::mat4 toWorld;
    
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> textures;
std::vector<int> faces;
std::vector<int> n_indices;
std::vector<int> t_indices;

void center();
glm::vec3 lineColor;
    
    
public:
	BoundingSphere(int r);
    ~BoundingSphere();
    
    void enlarge(float);
    void draw(GLuint);    
    glm::vec3 getCenter();
    float getRadius();
    void setModel(glm::mat4 world);
    glm::mat4 getModel();
    void setColor(glm::vec3 color);
    
    void parse(const char* filepath);

    void scale(float s);
    void translate(float x, float y, float z);
    
    bool checkIntersect( BoundingSphere* );
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    
};

#endif
