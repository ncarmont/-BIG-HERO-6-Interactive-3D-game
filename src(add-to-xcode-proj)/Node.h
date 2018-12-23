#ifndef _NODE_H_
#define _NODE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Node
{
public:
	virtual ~Node() {}
    virtual void draw(glm::mat4 C) = 0;
	virtual void update() = 0;
};

#endif
