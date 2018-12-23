#ifndef _BRANCH_H_
#define _BRANCH_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Branch
{
public:
	Branch(glm::vec3 pastPoint, glm::vec3 pointBeforeThat);
	~Branch();

	glm::mat4 toWorld;

    int rescursionNum;
	void draw(GLuint shader, float greenScale);
    void update();
//    std::vector <Subbranch*> subbranchesVec;
    glm::vec3 * getBranchPoints();
     glm::vec3 plantPoints[2];
//    void spin(float);

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
    
    glm::vec3 P0,P1,P2,P3,nextP0;
    glm::vec3 drawInRandomUpDirection(glm::vec3 p ,  glm::vec3 dirBranch);
    void rotateLeft(glm::vec3 pos);
};



#endif

