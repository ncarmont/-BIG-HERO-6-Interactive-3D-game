#include "Branch.h"
#include "Window.h"

Branch::Branch(glm::vec3 pastPoint, glm::vec3 pointBeforeThat)
{
    rescursionNum = 0;
    //Plant trunk/base
    P0 = pastPoint;
    P1 = pointBeforeThat;
    
    // Bernstein equation
    glm::vec3 plantPoints [2];
    plantPoints[0] = P0;
    plantPoints[1] = P1;  //drawInRandomUpDirection(P1, dirBranch);
	toWorld = glm::mat4(1.0f);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(plantPoints), plantPoints, GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

Branch::~Branch()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
}

void Branch::draw(GLuint shaderProgram, float greenScale)
{ 
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
    
    
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
    
    float greenValue = 0.30f;
    if(greenScale>=3.0f){
        greenValue = 0.12;
    } else if ( greenScale>=2.0f){
        greenValue = 0.24;
    };

    int vertexColorLocation = glGetUniformLocation(shaderProgram, "greenColorOrg");
    glUniform1f(vertexColorLocation, greenValue);
    
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

glm::vec3 Branch::drawInRandomUpDirection(glm::vec3 p,  glm::vec3 dirBranch){
    glm::vec3 newPoint;
    
    newPoint.x = -1.0f + (rand() / ( RAND_MAX / (5) ) );
    newPoint.y = 3.0f + (rand() / ( RAND_MAX / (5) ) );
    newPoint.z = -1.0f + (rand() / ( RAND_MAX / (5) ) );
    
    return newPoint;
}

glm::vec3 * Branch::getBranchPoints(){
    return plantPoints;
}

void Branch::rotateLeft(glm::vec3 pos){
    float angle = glm::radians(90.0f);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -pos);
    glm::mat4 rot = glm::rotate(glm::mat4(1),angle,glm::vec3(0.0f, 0.0f, 1.0f));
    toWorld= rot*translate*toWorld;
};


