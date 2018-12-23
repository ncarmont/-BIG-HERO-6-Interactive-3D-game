#include "Building.h"



Building::Building(GLint shaderProgram)
{
	//Save the shader
	shader = shaderProgram;

	//Get random number seeds
	seed = time(NULL);

	//Set the random number generator with the seed
	std::srand(seed);

	//Set the base dimensions
	base_width = 10.0f * 0.2f;
	base_length = 10.0f * 0.2f;

	num_rect = rand() % 5;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
}


Building::~Building()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
}

void Building::drawRect(float length, float width, float height) {
	//The vertices and normals of the rectangle
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	//Front face
	vertices.push_back(glm::vec3(length, height, 0.0f));
	vertices.push_back(glm::vec3(0.0f, height, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(length, height, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

	//Left face
	vertices.push_back(glm::vec3(0.0f, height, 0.0f));
	vertices.push_back(glm::vec3(0.0f, height, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));

	vertices.push_back(glm::vec3(0.0f, height, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	//Right face
	vertices.push_back(glm::vec3(length, height, width));
	vertices.push_back(glm::vec3(length, height, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(length, height, width));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, width));

	//Top face
	vertices.push_back(glm::vec3(length, height, width));
	vertices.push_back(glm::vec3(0.0f, height, width));
	vertices.push_back(glm::vec3(0.0f, height, 0.0f));

	vertices.push_back(glm::vec3(length, height, width));
	vertices.push_back(glm::vec3(0.0f, height, 0.0f));
	vertices.push_back(glm::vec3(length, height, 0.0f));

	//Bottom face
	vertices.push_back(glm::vec3(length, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	vertices.push_back(glm::vec3(length, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

	//Back face
	vertices.push_back(glm::vec3(0.0f, height, width));
	vertices.push_back(glm::vec3(length, height, width));
	vertices.push_back(glm::vec3(length, 0.0f, width));

	vertices.push_back(glm::vec3(0.0f, height, width));
	vertices.push_back(glm::vec3(length, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));

	//Calculate the normals for all the triangles
	for (int k = 0; k < vertices.size(); k += 3) {
		glm::vec3 edge1 = vertices[k + 1] - vertices[k];
		glm::vec3 edge2 = vertices[k + 2] - vertices[k + 1];
		normals.push_back(glm::cross(edge1, edge2));
		normals.push_back(glm::cross(edge1, edge2));
		normals.push_back(glm::cross(edge1, edge2));
	}

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Building::drawVersion1(glm::mat4 C) {
	

	//Use the shader for buildings
	glUseProgram(shader);

	//Get the model matrix location in the shader
	GLuint model_matrix = glGetUniformLocation(shader, "model");
	GLuint coloring = glGetUniformLocation(shader, "color_mode");
	glUniform1i(coloring, 1);

	//The size of the rectangle in the center (length and height are same)
	float rect_size = 4.0f * 0.2;

	//The maximum height of the buildings
	int max_height = 3;

	//Place the first rectangle in the center of the base
	glm::mat4 newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((base_width - rect_size), 0.0f, (base_length - rect_size))) * C;
	glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);

	//Draw the rectangle with the highest possible height
	this->drawRect(rect_size, rect_size, max_height);

	int num_rect = rand() % 3;
	for (int i = 0; i < num_rect; i++) {
		//Size dimensions of rectangle
		float length;
		float width;
		float height;

		//x and z location of the rectangle
		float x_location;
		float z_location;

		//The max units the rectangle can move within the base square
		int magnitude;

		//If city not scaled
//		length = (rand() % ((int)base_length - 2)) + 3;
//		width = (rand() % ((int)base_width - 2)) + 3;

		//When city scaled
		length = 0.2 * ((rand() % 9) + 1);
		width = 0.2 * ((rand() % 9) + 1);
		height = (rand() % max_height) + 1;

		//The x location of the rectangle
		magnitude = 10 * (float)(base_length - length);
		x_location = 0.1 * (rand() % (magnitude + 1));

		//The z location of the rectangle
		magnitude = 10 * (float)(base_width - width);
		z_location = 0.1 * (rand() % (magnitude + 1));

		//Pass the model matrix and draw the rectangle
		newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x_location, 0.0f, z_location)) * C;
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
		this->drawRect(length, width, height);
	}
}
//Round buildings from: http://www.shamusyoung.com/twentysidedtale/?p=2968
void Building::drawVersion2(glm::mat4 C) {
	//Use the shader for buildings
	glUseProgram(shader);

	//Get the model matrix location in the shader
	GLuint model_matrix = glGetUniformLocation(shader, "model");
	GLuint coloring = glGetUniformLocation(shader, "color_mode");
	glUniform1i(coloring, 2);

	float height;

	height = (rand() % 3) + 1;

	float angle = 0.0f;
	int right_counter = 0;

	glm::mat4 newMatrix = C;

	newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3((base_width / 2.0f), 0.0f, (base_length / 2.0f))) * newMatrix;

	int num_large = rand() % 4;

	while(angle < 360.0f) {
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);

		//Pass the model matrix and draw the rectangle
		this->drawRect((5.0f * 0.2f), (1.0f * 0.2f), height);

		int rotate = rand() % 2;

		//Regular rotation
		if (rotate == 1 && right_counter < num_large) {
			for (int i = 0; i < 10; i++) {
				glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
				//Pass the model matrix and draw the rectangle
				this->drawRect((2.5f * 0.2f), (1.0f * 0.2f), height);
				newMatrix = newMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			}
			angle = angle + 100.0f;
			right_counter++;
		}

		else{
			newMatrix = newMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			angle = angle + 5.0f;
		}
	}
}

//Buildings with different heights and a triangular roof on top
void Building::drawVersion3(glm::mat4 C) {

	//Use the shader for buildings
	glUseProgram(shader);

	//Get the model matrix location in the shader
	GLuint model_matrix = glGetUniformLocation(shader, "model");
	GLuint coloring = glGetUniformLocation(shader, "color_mode");

	float length;
	float width;
	float height;

	length = (rand() % ((int)base_length) + 1);
	width = (rand() % ((int)base_width) + 1);

	float prevHeight = 0.0f;

	glm::mat4 newMatrix = C;

	for (int i = 0; i < 2; i++) {
		height = (rand() % 2) + 1;

		//Pass the model matrix and draw the rectangle
		newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, prevHeight, 0.0f)) * C;
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
		glUniform1i(coloring, (i + 3));

		this->drawRect(length, width, height);

		prevHeight = prevHeight + height;
	}

	//Draw Triangular roof
	height = (rand() % 2) + 1;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	//Left
	vertices.push_back(glm::vec3((length / 2), height, (width / 2)));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	//Right
	vertices.push_back(glm::vec3((length / 2), height, (width / 2)));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, width));

	//Front
	vertices.push_back(glm::vec3((length / 2), height, (width / 2)));
	vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

	//Back
	vertices.push_back(glm::vec3((length / 2), height, (width / 2)));
	vertices.push_back(glm::vec3(length, 0.0f, width));
	vertices.push_back(glm::vec3(0.0f, 0.0f, width));

	//Calculate the normals for all the triangles
	for (int k = 0; k < vertices.size(); k += 3) {
		glm::vec3 edge1 = vertices[k + 1] - vertices[k];
		glm::vec3 edge2 = vertices[k + 2] - vertices[k + 1];
		normals.push_back(glm::cross(edge1, edge2));
		normals.push_back(glm::cross(edge1, edge2));
		normals.push_back(glm::cross(edge1, edge2));
	}

	//Pass the model matrix and draw the triangle
	newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, prevHeight, 0.0f)) * C;
	glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
	glUniform1i(coloring, 5);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int window = rand() % 3;
	if(window == 0){
		vertices.clear();

		float window_height = (rand() % (int)(prevHeight)) + 0.5f;

		vertices.push_back(glm::vec3((length / 2.0f), 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

		normals.clear();
		//Calculate the normals for all the triangles
		for (int k = 0; k < vertices.size(); k += 3) {
			glm::vec3 edge1 = vertices[k + 1] - vertices[k];
			glm::vec3 edge2 = vertices[k + 2] - vertices[k + 1];
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
		}

		//Pass the model matrix and draw the triangle
		newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, window_height, (width + 0.1f))) * C;
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
		glUniform1i(coloring, 6);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	else if (window == 1) {
		vertices.clear();

		float window_height = (rand() % (int)(prevHeight)) + 0.5f;

		vertices.push_back(glm::vec3(length, 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

		vertices.push_back(glm::vec3(length, 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		vertices.push_back(glm::vec3(length, 0.0f, 0.0f));

		normals.clear();
		//Calculate the normals for all the triangles
		for (int k = 0; k < vertices.size(); k += 3) {
			glm::vec3 edge1 = vertices[k + 1] - vertices[k];
			glm::vec3 edge2 = vertices[k + 2] - vertices[k + 1];
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
		}

		//Pass the model matrix and draw the triangle
		newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, window_height, (width + 0.1f))) * C;
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
		glUniform1i(coloring, 6);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	else {
		vertices.clear();

		float window_height = (rand() % (int)(prevHeight)) + 0.5f;

		vertices.push_back(glm::vec3((length / 2.0f), 0.5f, 0.0f));
		vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		vertices.push_back(glm::vec3((length / 2.0f), -0.5f, 0.0f));

		vertices.push_back(glm::vec3((length / 2.0f), -0.5f, 0.0f));
		vertices.push_back(glm::vec3(length, 0.0f, 0.0f));
		vertices.push_back(glm::vec3((length / 2.0f), 0.5f, 0.0f));

		normals.clear();
		//Calculate the normals for all the triangles
		for (int k = 0; k < vertices.size(); k += 3) {
			glm::vec3 edge1 = vertices[k + 1] - vertices[k];
			glm::vec3 edge2 = vertices[k + 2] - vertices[k + 1];
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
			normals.push_back(glm::cross(edge1, edge2));
		}

		//Pass the model matrix and draw the triangle
		newMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, window_height, (width + 0.1f))) * C;
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &newMatrix[0][0]);
		glUniform1i(coloring, 6);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Building::draw(glm::mat4 C) {}

void Building::update() {
	//Update the seed
	seed = time(NULL);
}
