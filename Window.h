#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"
#include <time.h>

//ellens files
#include "Node.h"
#include "Terrain.h"
#include "Building.h"
#include "City.h"


//hills existing files
#include "OBJObject.h"
#include "SkyBox.h"
#include "Patch.h"
#include "Surface.h"
#include "Ocean.h"
#include "Mochi.h"
#include "Baymax.h"
#include "Forest.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Window
{
public:
    //window
	static int width;
	static int height;
    static glm::mat4 P; // P for projection
    static glm::mat4 V; // V for view
    static float city_size;
    
    //mouse
    static glm::vec3 lastPos;
    static glm::vec3 currPos;
    static bool left_button_start;
    static bool left_button_pressed;
    
    
	//functions
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    
    static glm::vec3 trackball( double x, double y, int width,int height);
    static void rotateCam(float angle, glm::vec3 axis);
    static void translateCam(float x, float y, float z);
};


#endif
