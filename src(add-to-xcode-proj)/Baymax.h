#ifndef BAYMAX_H
#define BAYMAX_H

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

#include "BoundingSphere.h"

class Baymax
{
private:
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> textures;
std::vector<int> faces;
std::vector<int> n_indices;
std::vector<int> t_indices;
glm::mat4 toWorld;

float angle;

public:
	Baymax(const char* filepath);
    ~Baymax();

    glm::mat4 getModel();
    int getFurthest();
    
	void parse(const char* filepath);
    void draw(GLuint);
    void center();
    void scale(float s);
    void translate(float x, float y, float z);
    void rotate();
    
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    void loadTexture();
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, VBON, VBOT;
    
};

#endif
