#ifndef _CITY_H
#define _CITY_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "Building.h"
#include "Node.h"

class City : public Node
{
private:
/* The number of blocks in one row and the array modeling city structure
	   The park is located wherever structure[x][y] = 1, otherwise block of buildings */
	int num_blocks = 3;
	bool structure[3][3];

	//Buildings are 10x10, roads are 3 units wide
	float building_size = 10.0f * 0.2f;
	float street_size = 1.5f * 0.2f;

	/* The size of each block
   Starting from top left corner:
   Half-Road Building Road Building Half-Road
   Half-Road Building Road Building Half-Road */
    float size = (2 * building_size) + (4 * street_size);

	//Set the vertices of a block
	glm::vec3 block_vertices[6] = 
	{
		glm::vec3(size, 0.0f, size),
		glm::vec3(0.0f, 0.0f, size),
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(size, 0.0f, size),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(size, 0.0f, 0.0f)
	};

	//The random number generator seed for park location
	unsigned int seed;

	Building * building;

	GLint shader;

public:
	City(GLint city_shader, GLint building_shader);
	~City();

	void setStructure();

	void draw(glm::mat4 C);
	void drawBlock(glm::mat4 C, bool park);
	void update();

	GLuint VBO, VAO;

};

#endif

