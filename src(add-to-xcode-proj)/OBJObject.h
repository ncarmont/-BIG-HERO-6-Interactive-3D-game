#ifndef OBJOBJECT_H
#define OBJOBJECT_H

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

class OBJObject
{
private:
//std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<glm::vec2> textures;
std::vector<int> faces;
std::vector<int> n_indices;
std::vector<int> t_indices;
glm::mat4 toWorld;
float angle;

public:
	OBJObject(const char* filepath);
    ~OBJObject();
    
    void setModel(glm::mat4 model);
	void parse(const char* filepath);
	//void draw(float p_size);
    void draw(GLuint);
    void center();
    glm::vec3 getCenter();
	void update();
    void move();
    void scale(float s);
	void spin(float deg);
    //void rotate(float angle, glm::vec3 axis);
    void translate(float x, float y, float z);
    void translate(glm::vec3 v);
    
    unsigned char* loadPPM(const char* filename, int& width, int& height);
    void loadTexture();
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, VBON, VBOT;
    GLuint uProjection, uModelview, uScaler;
    
};

#endif
