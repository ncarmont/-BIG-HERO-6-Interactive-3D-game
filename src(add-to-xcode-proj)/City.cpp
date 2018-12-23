#include "City.h"
City::City(GLint city_shader, GLint building_shader)
{
	//Set city's and building's shader
	shader = city_shader;

	//Set the city's structure array
	setStructure();

	//Create a new Building object to draw buildings
	building = new Building(building_shader);

	//Generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &block_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

City::~City()
{
	//Delete the building object, VAOs and VBOs
	delete(building);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void City::setStructure() {
	//Set the random number generator seed to the current time
	seed = time(NULL);

	//Set all values in the structure array to false
	for (int i = 0; i < num_blocks; i++) {
		for (int j = 0; j < num_blocks; j++) {
			structure[i][j] = false;
		}
	}

	//Get a random number for the starting location of the park
	std::srand(seed);
	int x_park = rand() % num_blocks;
	int y_park = rand() % num_blocks;
  

	//Set the starting location of the park
	structure[x_park][y_park] = true;

	//Get a random number to determine the orientation of the park (horizontal or vertical)
	int park_rand = rand() % 2;

	//Set a horizontal orientation that fits within the city
	if (park_rand == 0) {
		if (x_park - 1 >= 0) {
			structure[x_park - 1][y_park] = true;
		}
		else {
			structure[x_park + 1][y_park] = true;
		}
	}

	//Set a vertical orientation that fits within the city
	else if (park_rand == 1) {
		if (y_park - 1 >= 0) {
			structure[x_park][y_park - 1] = true;
		}
		else {
			structure[x_park][y_park + 1] = true;
		}
	}
}

void City::draw(glm::mat4 C) {
	//Set the random number generator with the seed
	std::srand(seed);
    
	
	//Go through city structure
	for (int i = 0; i < num_blocks; i++) {
		for (int j = 0; j < num_blocks; j++) {

			//Use the city's shader
			glUseProgram(shader);

			//Get the model matrix and coloring location in the shader
			GLuint model_matrix = glGetUniformLocation(shader, "model");
			GLuint color = glGetUniformLocation(shader, "coloring");

			//Where the block starts
			float x_offset = size * i;
			float z_offset = size * j;

			//Matrix to store translation matrices
			glm::mat4 posMatrix = C * glm::translate(glm::mat4(1.0f), glm::vec3(x_offset, 0.0f, z_offset));
			glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &posMatrix[0][0]);

			//If location of park is found
			if (structure[i][j] == true) {
				//Set the color of the park
				glUniform1i(color, 1);				
                drawBlock(glm::mat4(1.0f), true);
			}

			//Else draw a block
			else{
				//Set coloring of the streets
				glUniform1i(color, 0);
				drawBlock(posMatrix, false);
			}
		}
	}
}

void City::drawBlock(glm::mat4 C, bool park) {

	//Draw the block representing streets
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &block_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Draw the Buildings
	if (park == false) {
		int cylinder = 0;

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {

				int building_type = (rand() % 3) + 1;

				float x_translate = (i * building_size) + ((i + 1) * street_size);
				float z_translate = (j * building_size) + ((j + 1) * street_size);				
			
				if (building_type == 1) {
					building->drawVersion1(glm::translate(glm::mat4(1.0f), glm::vec3(x_translate, 0.0f, z_translate)) * C);
				}

				else if (building_type == 2 && cylinder < 1) {
					building->drawVersion2(glm::translate(glm::mat4(1.0f), glm::vec3(x_translate, 0.0f, z_translate)) * C);
					cylinder++;
				}
				
				else {
					building->drawVersion3(glm::translate(glm::mat4(1.0f), glm::vec3(x_translate, 0.0f, z_translate)) * C);
				}
			}
		}
	}

}

void City::update() {
	//Set the city's structure with new random locations
	setStructure();
}
