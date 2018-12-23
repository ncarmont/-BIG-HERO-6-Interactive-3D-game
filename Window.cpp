#include "Window.h"


//#include <stdio.h>
//#include <irrKlang.h>
//using namespace irrklang;
//
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll



const char* window_title = "GLFW Starter Project";


/*** define shader paths ***/
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"
#define VERTEX_SKYBOX "shader_skybox.vert"
#define FRAGMENT_SKYBOX "shader_skybox.frag"
#define VERTEX_PATCH "shader_patch.vert"
#define FRAGMENT_PATCH "shader_patch.frag"
#define VERTEX_TOON "shader_toon.vert"
#define FRAGMENT_TOON "shader_toon.frag"
#define VERTEX_TOON_TEX "shader_toon_tex.vert"
#define FRAGMENT_TOON_TEX "shader_toon_tex.frag"
#define VERTEX_TERRAIN "shader_terrain.vert"
#define FRAGMENT_TERRAIN "shader_terrain.frag"
#define VERTEX_BAYMAX "shader_baymax.vert"
#define FRAGMENT_BAYMAX "shader_baymax.frag"
#define VERTEX_BOUNDING "shader_bounding.vert"
#define FRAGMENT_BOUNDING "shader_bounding.frag"
#define VERTEX_CITY "shader_city.vert"
#define FRAGMENT_CITY "shader_city.frag"
#define VERTEX_FOREST "shader_tree.vert"
#define FRAGMENT_FOREST "shader_tree.frag"
#define VERTEX_BUILDING "shader_building.vert"
#define FRAGMENT_BUILDING "shader_building.frag"

/*** declare shaders ***/
GLint shaderProgram;
GLint shaderSkybox;
GLint shaderPatch;
GLint shaderToon;
GLint shaderToonTex;
GLint shaderTerrain;
GLint shaderBaymax;
GLint shaderBounding;
GLint shaderCity;
GLint shaderForest;
GLint shaderBuilding;



/*** declare objects to be shown ***/
SkyBox * skybox; //will use soon
Mochi * mochi;
Baymax * baymax;            //must be rendered with toonbaymax bc of normals
Terrain * terrain;
Ocean * ocean;              //4 bezier patches, c1 continuity
City * city;
Forest * forest;

BoundingSphere * bs_baymax; //baymax bounding sphere
BoundingSphere * bs_mochi;  //mochi bounding sphere

/*** current unused objects ***/
OBJObject * bunny; //used for toon shading testing, more obvious
//Patch * water; //patch -- 1 square
//Surface * ocean; //surface -- 2 squares


/*** booleans for keyboard toggling ***/
bool isBaymax;   //1
bool isMochi;    //2
bool isBounding; //3
bool isOcean;    //4
bool isTerrain;  //5
bool isCity;     //6
bool isBuilding; //7
bool isPlant;    //8
bool isObject;   //0, for testing others
bool z; //toggles whether up/down keys translates z or y axis
bool isCamera;    //p
bool isRandom;    //9
bool isWaterOnly; //O


/*** hard code scaling constant ***/
float scale = 2.0f;
float fov = 45.0f;


/*** static variables for mouse drag movements (camera rotation) ***/
bool Window::left_button_start;
bool Window::left_button_pressed;
glm::vec3 Window::currPos;
glm::vec3 Window::lastPos;



/*** default camera parameters ***/
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);     // e  | camera position
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);  // d  | point camera is looking at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);       // up | camera up vector


/*** light settings ***/
//mine, should sync with ellens
glm::vec3 light_pos(0.0f, 10.0f, 10.0f);
glm::vec3 light_intensity(3.0f, 3.0f, 3.0f);
//ellens light settings
glm::vec3 direc_lightPos(50.0f,50.0f,0.0f);
int coloring = 0;


/*** Window static variables ***/
int Window::width;
int Window::height;

glm::mat4 Window::P; //projection matrix
glm::mat4 Window::V; //view matrix


/*** translation matrices ***/
glm::mat4 totalTransform;
glm::mat4 scaledDownCity;

float Window::city_size;


/************** BEGIN CODE **************/
 
void Window::initialize_objects()
{
//    ISoundEngine* engine = createIrrKlangDevice();

    
    /*** initialize static mouse variables ***/
    left_button_start = false;
    left_button_pressed = false;
    currPos = glm::vec3(0);
    lastPos = glm::vec3(0);

    
    /*** load shaders ***/
    shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    shaderSkybox = LoadShaders(VERTEX_SKYBOX, FRAGMENT_SKYBOX);
    shaderPatch = LoadShaders(VERTEX_PATCH, FRAGMENT_PATCH);
    shaderToon = LoadShaders(VERTEX_TOON,FRAGMENT_TOON);
    shaderToonTex = LoadShaders(VERTEX_TOON_TEX, FRAGMENT_TOON_TEX);
    shaderTerrain = LoadShaders(VERTEX_TERRAIN,FRAGMENT_TERRAIN);
    shaderBaymax = LoadShaders(VERTEX_BAYMAX,FRAGMENT_BAYMAX);
    shaderBounding = LoadShaders(VERTEX_BOUNDING,FRAGMENT_BOUNDING);
    shaderCity = LoadShaders(VERTEX_CITY,FRAGMENT_CITY);
    shaderForest = LoadShaders(VERTEX_FOREST,FRAGMENT_FOREST);
    shaderBuilding = LoadShaders(VERTEX_BUILDING, FRAGMENT_BUILDING);

    /*** initialize objects ***/
    baymax = new Baymax("baymax.obj");
    mochi = new Mochi("mochi.obj");
    ocean = new Ocean();
    
    glm::vec3 startPoint = glm::vec3(-2.0f,-4.0f,1.0f);
    std::vector <glm::vec3> terrainPos;
    std::srand(time(NULL));
    for (int i = 0; i < 30; i++){
        float x = (rand() % 12) * 1.0f;
        float z = (rand() % 12) * 1.0f;
        glm::vec3 temp = glm::vec3(x,-1.0f,z);
        terrainPos.push_back(temp);
    }

//    for (int i = 0; i<5; i++){
//        float rand1 = 40.0f*(rand()/ double(RAND_MAX))-20.0f;
//        float rand3 =40.0f*(rand()/ double(RAND_MAX))-20.0f;
//        glm::vec3 rand = glm::vec3(rand1,-5.0f,rand3);
//        terrainPos.push_back(rand);
//    };
    forest = new Forest(glm::mat4(1),startPoint, "F[-B]F[+B][B]", 3, 1.3f, 0.1f, terrainPos);
    
    terrain = new Terrain(shaderTerrain);
    city = new City(shaderCity,shaderBuilding);
    //    city = new City(shaderCity,shaderTerrain, shaderForest);

    //create bounding spheres with longest radius of object
    bs_baymax = new BoundingSphere(baymax->getFurthest());
    bs_mochi = new BoundingSphere(mochi->getFurthest());
    
    //currently unused
    bunny = new OBJObject("bunny.obj");
    skybox = new SkyBox();
//    water = new Patch();
//    ocean = new Surface();
    
    
    /*** hardcode initial scaling to fit relatively straight to camera ***/
    baymax->scale(1/(scale*4));             //baymax too big
    mochi->scale(1/(scale*4));              //mochi too big
    baymax->translate(-0.5f,0.1f,-1.75f);
    mochi->translate(2.0f,-0.5f,0.0f);   //move mochi to right so we can see baymax
    bs_mochi->scale(1/(scale*4));           //center of bs moves to match scaling
    bs_baymax->scale(1/(scale*4));
    ocean->scale(scale*6);                //make ocean bigger
    ocean->translate(0.0f, -4.0f, 0.0f);//move ocean downwards
    
    
    //randomize mochis position after
    mochi->reset();
    mochi->scale(1/(scale*4));
    mochi->randomize();

    
    /*******************************************/
    /*******************************************/
    
    //terrain/city/buildings translations
    
    Window::city_size = 0.1;
    
    //scale down city
    glm::mat4 scalingCity = glm::scale(glm::mat4(1.0f), glm::vec3(Window::city_size)); //.2
    
    //translate
    glm::vec3 translationAmount = glm::vec3(-10.0f, -5.0f, -10.0f);
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), translationAmount);
    glm::mat4 translationCity = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.99f, -1.0f));
    
    glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f); //rotate around y
    float angle = 180.0f; //90 at a time
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle / 180.0f * glm::pi<float>(), axis);
    
//    scaledDownCity = translationCity*scalingCity*glm::mat4(1.0f);
    scaledDownCity = translationCity*glm::mat4(1.0f);
    totalTransform = translationMat*32.0f; //*scaling*matrix*
    
    
    /*******************************************/
    
    
    /*** initialize toggle bools ***/
    isBaymax = false;
    isMochi = false;
    isTerrain = false;
    isOcean = false;
    isObject = false;
    isBounding = false;
    isCity = false;
    isPlant = false;
    z = false;
    isCamera = false;
    isRandom = false;
    isWaterOnly = false;

}


// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    glDeleteProgram(shaderPatch);
    glDeleteProgram(shaderSkybox);
	glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderToon);
    glDeleteProgram(shaderToonTex);
    glDeleteProgram(shaderTerrain);
    glDeleteProgram(shaderBaymax);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
    
}

void Window::idle_callback()
{

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, GL_FALSE, &cam_pos[0]);
    
    /*** sends camera and positions to shaders, can remove some later ***/
    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, &cam_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &light_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, &light_intensity[0]);
    
    glUseProgram(shaderToonTex);
    glUniform3fv(glGetUniformLocation(shaderToonTex, "cameraPos"), 1, &cam_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderToonTex, "lightPos"), 1, &direc_lightPos[0]);
    glUniform3fv(glGetUniformLocation(shaderToonTex, "lightColor"), 1, &light_intensity[0]);
    
    glUseProgram(shaderToon);
    glUniform3fv(glGetUniformLocation(shaderToon, "cameraPos"), 1, &cam_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderToon, "lightPos"), 1, &light_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderToon, "lightColor"), 1, &light_intensity[0]);
    
    glUseProgram(shaderBaymax);
    glUniform3fv(glGetUniformLocation(shaderBaymax, "cameraPos"), 1, &cam_pos[0]);
    glUniform3fv(glGetUniformLocation(shaderBaymax, "lightPos"), 1, &direc_lightPos[0]);
    glUniform3fv(glGetUniformLocation(shaderBaymax, "lightColor"), 1, &light_intensity[0]);

    
    /********** BEGIN ELLENS TERRAIN SHADING VALUES *********/
    /*** passing lighting stuff to terrain, can possibly clean up ***/
    glUseProgram(shaderTerrain);
    
    //Pass the camera position to the shader program
    glUniform3fv(glGetUniformLocation(shaderTerrain, "cameraPos"), 1, &cam_pos[0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderTerrain, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderTerrain, "view"), 1, GL_FALSE, &V[0][0]);
    
    //lighting
    glUniform3fv(glGetUniformLocation(shaderTerrain, "light.d_light_pos"), 1, &direc_lightPos[0]);
    glUniform3f(glGetUniformLocation(shaderTerrain, "light.d_light_color"), 1.0f, 1.0f, 1.0f);
    GLint shininess = glGetUniformLocation(shaderTerrain, "shininess");
    //color vertices
    glUniform1f(shininess, 10.0f);
    
    glUseProgram(shaderCity);
    glUniformMatrix4fv(glGetUniformLocation(shaderCity, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderCity, "view"), 1, GL_FALSE, &V[0][0]);
   
    glUseProgram(shaderBuilding);
    glUniform3fv(glGetUniformLocation(shaderBuilding, "cameraPos"), 1, &cam_pos[0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderBuilding, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderBuilding, "view"), 1, GL_FALSE, &V[0][0]);
    glUniform3fv(glGetUniformLocation(shaderBuilding, "light.d_light_pos"), 1, &direc_lightPos[0]);
    glUniform3f(glGetUniformLocation(shaderBuilding, "light.d_light_color"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderBuilding, "shininess"), 10.0f);
    
    
    /********** END ELLENS TERRAIN SHADING VALUES *********/
    
    
    
    /********** BEGIN DRAWING *********/
  
    if (isWaterOnly){
        glUseProgram(shaderPatch);
        ocean->draw(shaderPatch);
        glfwPollEvents();
        glfwSwapBuffers(window);
        return;
    }

    //if boundingCircles are on, check if they intersect and color appropriately
    if (isBounding){ //COLLISION!!
        if ( bs_baymax->checkIntersect(bs_mochi) == true ){
            bs_baymax->setColor(glm::vec3(1.0f,0.0f,0.0f));
            bs_mochi->setColor(glm::vec3(1.0f,0.0f,0.0f));
            
            if (isRandom){
                mochi->reset();
                mochi->scale(1/(scale*4));
                mochi->randomize();
            }
        }else{
            bs_baymax->setColor(glm::vec3(0.0f,0.0f,0.0f));
            bs_mochi->setColor(glm::vec3(0.0f,0.0f,0.0f));
        }
        glUseProgram(shaderBounding);
        bs_baymax->setModel(baymax->getModel());
        bs_baymax->draw(shaderBounding);
        bs_mochi->setModel(mochi->getModel());
        bs_mochi->draw(shaderBounding);
    }else{ //no bounding box but random is on
//        if ( bs_baymax->checkIntersect(bs_mochi) == true ){
//            if (isRandom){
//                mochi->reset();
//                mochi->scale(1/(scale*4));
//                mochi->randomize();
//            }
//        }
    }

    glUseProgram(shaderSkybox);
    skybox->draw(shaderSkybox);

    glUseProgram(shaderPatch);
    ocean->draw(shaderPatch);

    glUseProgram(shaderBaymax);
    baymax->draw(shaderBaymax);
//
    glUseProgram(shaderToonTex);
    mochi->draw(shaderToonTex);
    //    glUseProgram(shaderProgram);
    //    mochi->draw(shaderProgram);
    
    glUseProgram(shaderForest);
    forest->draw(shaderForest);

    
    
    glUseProgram(shaderTerrain);
    terrain->draw(totalTransform);
//
    if (isObject){
        glUseProgram(shaderToon);
        bunny->draw(shaderToon);
    }
//    glUseProgram(shaderBuilding);
    glUseProgram(shaderCity);
    city->draw(scaledDownCity);
    
   

    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
    
}




/*
 1: baymax
 2: mochi
 3: bounding circles
 4: ocean
 5: terrain
 6: city
 7: buildings
 8: plants
 0: test object
 */

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        else if (key == GLFW_KEY_1){ //BAYMAX
            isBaymax = !isBaymax;
        }else if (key == GLFW_KEY_2){ //MOCHI
            isMochi = !isMochi;
        }else if (key == GLFW_KEY_3){ //BOUNDING
            isBounding = !isBounding;
        }else if (key == GLFW_KEY_4){ //OCEAN
            isOcean = !isOcean;
        }else if (key == GLFW_KEY_5){ //TERRAIN
            isTerrain = !isTerrain;
        }else if (key == GLFW_KEY_6){ //CITY
            isCity = !isCity;
        }else if (key == GLFW_KEY_7){ //BUILDING
            isBuilding = !isBuilding;
        }else if (key == GLFW_KEY_8){ //PLANT
            isPlant = !isPlant;
        }else if (key == GLFW_KEY_0){ //OBJECT TEST (bunny currently)
            isObject = !isObject;
        }else if (key == GLFW_KEY_Z){ //whether up down keys go y or z axis
            z = !z;
        }else if (key == GLFW_KEY_P){ //whether up down keys go y or z axis
            isCamera = !isCamera;
        }else if (key == GLFW_KEY_9){ //whether up down keys go y or z axis
            isRandom = !isRandom;
        }else if (key == GLFW_KEY_O){ //whether up down keys go y or z axis
            isWaterOnly = !isWaterOnly;
        }
    
        else if (key == GLFW_KEY_C){
            if(mods == GLFW_MOD_SHIFT){
                fov+=5;
            }else{
                fov-=5;
            }
            P = glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 1000.0f);
        }
        
        /*** SCALE ***/
        else if (key == GLFW_KEY_S){
            if(mods == GLFW_MOD_SHIFT){ //uppercase s scales up
                if (isOcean){    ocean->scale(scale);  }
                if (isBaymax){   baymax->scale(scale);
                                 bs_baymax->scale(scale); }
                if (isMochi){    mochi->scale(scale);
                                 bs_mochi->scale(scale);  }
                if (isObject){ bunny->scale(scale);  }
//                if (isTerrain){  terrain->scale(scale);  }
            }else{               //lowercase s scales down
                if (isOcean){    ocean->scale(1/scale);  }
                if (isBaymax){   baymax->scale(1/scale);
                                 bs_baymax->scale(1/scale); }
                if (isMochi){    mochi->scale(1/scale);
                                 bs_mochi->scale(1/scale); }
                if (isObject){  bunny->scale(1/scale);  }
//                if (isTerrain){  terrain->scale(1/scale);  }
            }
        }
        /*** REGENERATE/ROTATE ***/
        else if (key == GLFW_KEY_R){
            if (isOcean){   ocean->regenerate();  }
            if (isTerrain){ terrain->update();  }
            if (isCity) { city->update(); }
            if (isBaymax){  baymax->rotate();  }
            if (isMochi) {
                mochi->reset();
                mochi->scale(1/(scale*4));
                mochi->randomize();
            }
            if (isPlant){
                std::srand(time(NULL));
                int type = (rand() % 3);
                std::cout << "TYPE IS: " << type << std::endl;
                if (type == 0){
                    forest->update("F[-B]F[+B][B]",1.3f, 0.1f);
                }else if(type == 1){
                    forest->update("F-[[B]+B]+F[+FB]-B", 1.9f, 0.3f);
                }else if(type == 2){
                    forest->update("F[+B][-B]FB", 1.4f,0.3f);
                }
            }
        }
//        /*** ROTATE ***/
//        else if (key == GLFW_KEY_T){
////            if (isTerrain){  terrain->rotate();  }
//            if (isBaymax){  terrain->rotate();  }
//        }
        /*** PRINT CENTERS, can delete after ***/
        else if (key == GLFW_KEY_P){
//            glm::vec3 t = terrain->getCenter();
            glm::vec3 o = ocean->getCenter();
//            printf("terrain (%f,%f,%f)\n", t.x, t.y, t.z);
            printf("ocean (%f,%f,%f)\n", o.x, o.y, o.z);
        }
        /*** TRANSLATIONS ***/
        else if (key == GLFW_KEY_LEFT){ //move -x
            if (isOcean){   ocean->translate(-1.0f, 0.0f, 0.0f);  }
            if (isBaymax){  baymax->translate(-0.5f, 0.0f, 0.0f); }
            if (isMochi){   mochi->translate(-0.5f, 0.0f, 0.0f);  }
            if (isCamera){  translateCam(-1.0f, 0.0f, 0.0f); }
//            if (isTerrain){ terrain->translate(-1.0f, 0.0f, 0.0f); }
            if (isObject){ bunny->translate(-1.0f, 0.0f, 0.0f);  }
        }
        else if (key == GLFW_KEY_RIGHT){ //move +x
            if (isOcean){   ocean->translate(1.0f, 0.0f, 0.0f);  }
            if (isBaymax){  baymax->translate(0.5f, 0.0f, 0.0f); }
            if (isMochi){   mochi->translate(0.5f, 0.0f, 0.0f);  }
            if (isCamera){  translateCam(1.0f, 0.0f, 0.0f); }
//            if (isTerrain){ terrain->translate(1.0f, 0.0f, 0.0f); }
            if (isObject){ bunny->translate(1.0f, 0.0f, 0.0f); }
        }
        else if (key == GLFW_KEY_UP){ //move +y or -z depending on if (z) is toggled
            if (!z){ //move -z
                if (isOcean) {  ocean->translate(0.0f, 0.0f, -1.0f);  }
                if (isBaymax){  baymax->translate(0.0f, 0.0f, -0.5f); }
                if (isMochi) {  mochi->translate(0.0f, 0.0f, -0.5f);  }
                if (isCamera){  translateCam(0.0f, 0.0f, -1.0f); }
//                if (isTerrain){ terrain->translate(0.0f, 0.0f, -1.0f);  }
                if (isObject){ bunny->translate(0.0f, 0.0f, -1.0f);  }
            }else{ //move +y
                if (isOcean) {  ocean->translate(0.0f, 1.0f, 0.0f);  }
//                if (isBaymax){  baymax->translate(0.0f, 0.5f, 0.0f); }
//                if (isMochi) {  mochi->translate(0.0f, 0.5f, 0.0f);  }
                if (isCamera){  translateCam(0.0f, 1.0f, 0.0f); }
//                if (isTerrain){ terrain->translate(0.0f, 1.0f, 0.0f);  }
                if (isObject){ bunny->translate(0.0f, 1.0f, 0.0f);  }
            }
        }
        else if (key == GLFW_KEY_DOWN){ //move -y or +z depending on if (z) is toggled
            if (!z){
                if (isOcean) {  ocean->translate(0.0f, 0.0f, 1.0f);  }
                if (isBaymax){  baymax->translate(0.0f, 0.0f, 0.5f); }
                if (isMochi) {  mochi->translate(0.0f, 0.0f, 0.5f);  }
                if (isCamera){  translateCam(0.0f, 0.0f, 1.0f); }
//                if (isTerrain){ terrain->translate(0.0f, 0.0f, 1.0f);  }
                if (isObject){ bunny->translate(0.0f, 0.0f, 1.0f);  }
            }else{ //y
                if (isOcean) {  ocean->translate(0.0f, -1.0f, 0.0f);  }
//                if (isBaymax){  baymax->translate(0.0f, -0.5f, 0.0f); }
//                if (isMochi) {  mochi->translate(0.0f, -0.5f, 0.0f);  }
                if (isCamera){  translateCam(0.0f, -1.0f, 0.0f); }
//                if (isTerrain){ terrain->translate(0.0f, -1.0f, 0.0f);  }
                if (isObject){ bunny->translate(0.0f, -1.0f, 0.0f);  }
            }
        }
        /******** CAMERA SETTINGS ********/
        else if(key == GLFW_KEY_F1){
            //original cam settings
            cam_pos = glm::vec3(0.0f, 0.0f, 20.0f);     // e  | camera position
            cam_look_at = glm::vec3(0.0f, 0.0f, 0.0f);  // d  | point camera is looking at
            cam_up = glm::vec3(0.0f, 1.0f, 0.0f);       // up | camera up vector
            Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if(key == GLFW_KEY_F2){
            ////top down cam settings
            cam_pos = glm::vec3(5.0f, 20.0f, 20.0f);     // e  | camera position
            cam_look_at = glm::vec3(5.0f, 0.0f, 5.0f);  // d  | point camera is looking at
            cam_up = glm::vec3(0.0f, 1.0f, 0.0f);       // up | camera up vector
            Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
  
        
        display_callback(window);
	}
}




void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS){
            //record curr
            left_button_start = true;
            left_button_pressed = true;
        }
        else if(action == GLFW_RELEASE){
            //record letgo
            left_button_pressed = false;
       }
    }
}


void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
    if (left_button_start){
        lastPos = trackball(xpos, ypos, width, height);
        left_button_start = false;
    } else if (left_button_pressed){
        currPos = trackball(xpos, ypos, width, height);
        glm::vec3 rot_axis = glm::cross( Window::lastPos, Window::currPos );
        float rot_angle = glm::length((currPos-lastPos)) * 100.0f; //length of distance is velocity
        lastPos = currPos;
        rotateCam(rot_angle, rot_axis);
//        Window::resize_callback(window, width, height);
    }
}


glm::vec3 Window::trackball( double x, double y, int width, int height){
    glm::vec3 v;     // Vector v is the synthesized 3D position of the mouse location on the trackball
    float d;         // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
    v.x = (2.0 * x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
    v.y = (height - 2.0 * y) / height;   // this does the equivalent to the above for the mouse Y position
    v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
    d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
    d = (d<1.0) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
    v.z = sqrtf(1.001 - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
    glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;
}


void Window::rotateCam(float angle, glm::vec3 axis){
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle / 180.0f * glm::pi<float>(), axis);
    cam_pos = matrix * glm::vec4(cam_pos,1.0f);
//    cam_look_at = matrix * glm::vec4(cam_look_at,1.0f);
//    cam_up = matrix * glm::vec4(cam_up,1.0f);
    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    light_pos = cam_pos;
}

void Window::translateCam(float x, float y, float z){
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    cam_pos = translationMat * glm::vec4(cam_pos,1.0f);
    cam_look_at = translationMat * glm::vec4(cam_look_at,1.0f);
    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}


