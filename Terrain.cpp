#include "Terrain.h"

Terrain::Terrain(GLint shaderProgram)
{
    shader = shaderProgram;
    seed = time(NULL);
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    
    this->setData();
}

Terrain::~Terrain()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
}

void Terrain::setData() {
    std::srand(seed);
    
    for (int j = 0; j < TOTAL; j++) {
        for (int i = 0; i < TOTAL; i++) {
            glm::vec3 init_vertex = glm::vec3(i, INT_MIN, j);
            vertices.push_back(init_vertex);
        }
    }
    
    int height_limit = 5;
    int magnitude = 20;
    
    vertices[0].y = rand() % height_limit;
    vertices[TOTAL - 1].y = rand() % height_limit;
    vertices[TOTAL * (TOTAL - 1)].y = rand() % height_limit;
    vertices[TOTAL * (TOTAL - 1) + (TOTAL - 1)].y = rand() % height_limit;
    
    diamond_step(pow(2, N - 1), pow(2, N - 1), TOTAL / 2, magnitude);
    
    for(int i = 0; i < TOTAL; i++){
        vertices[i].y = 0.0f;
        vertices[(TOTAL * i)].y = 0.0f;
        vertices[(TOTAL * (TOTAL-1)) + i].y = 0.0f;
        vertices[(TOTAL * i) + (TOTAL-1)].y = 0.0f;
    }
    
    for(int i = 7; i < 25; i++){
        for(int j = 7; j < 25; j++){
            vertices[(TOTAL * j) + i].y = 4.0f;
        }
    }
    
    //Order the vertices to create triangles
    for (int j = 1; j < (TOTAL * (TOTAL - 1)); j++) {
        if (j%TOTAL == 0) {
            continue;
        }
        
        int bottom_left = (j - 1) + TOTAL;
        
        //First triangle
        ordered.push_back(vertices[j]);
        ordered.push_back(vertices[j - 1]);
        ordered.push_back(vertices[bottom_left]);
        
        //Second triangle
        ordered.push_back(vertices[j]);
        ordered.push_back(vertices[bottom_left]);
        ordered.push_back(vertices[bottom_left + 1]);
    }
    
    vertices.clear();
    
    //Find the normals for all triangles
    for (int k = 0; k < ordered.size(); k += 3) {
        glm::vec3 edge1 = ordered[k + 1] - ordered[k];
        glm::vec3 edge2 = ordered[k + 2] - ordered[k + 1];
        normals.push_back(glm::cross(edge1, edge2));
        normals.push_back(glm::cross(edge1, edge2));
        normals.push_back(glm::cross(edge1, edge2));
    }
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, ordered.size() * sizeof(glm::vec3), ordered.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::diamond_step(int x, int z, int step, int magnitude) {
    int x_minus = x - step;
    int x_plus = x + step;
    int z_minus = z - step;
    int z_plus = z + step;
    
    if (vertices[(TOTAL * z) + x].y == INT_MIN) {
        float corner1 = vertices[(TOTAL * z_minus) + x_minus].y;
        float corner2 = vertices[(TOTAL * z_minus) + x_plus].y;
        float corner3 = vertices[(TOTAL * z_plus) + x_minus].y;
        float corner4 = vertices[(TOTAL * z_plus) + x_plus].y;
        
        if (corner1 == INT_MIN || corner2 == INT_MIN || corner3 == INT_MIN || corner4 == INT_MIN) {
            return;
        }
        
        float offset = (std::rand() % magnitude) + 1;
        float value = (offset * 0.1f) - (magnitude * 0.1f / 2.0f);
        vertices[(TOTAL * z) + x].y = ((corner1 + corner2 + corner3 + corner4) / 4) + (value);
    }
    
    if (magnitude - 2 != 0) {
        magnitude = magnitude - 2;
    }
    else {
        magnitude = 1;
    }
    
    if (x_minus >= 0) {
        square_step(x_minus, z, step, magnitude);
    }
    
    if (x_plus < TOTAL) {
        square_step(x_plus, z, step, magnitude);
    }
    
    if (z_minus >= 0) {
        square_step(x, z_minus, step, magnitude);
    }
    
    if (z_plus < TOTAL) {
        square_step(x, z_plus, step, magnitude);
    }
}

void Terrain::square_step(int x, int z, int step, int magnitude) {
    int counter = 0;
    
    float corner1 = 0.0f;
    float corner2 = 0.0f;
    float corner3 = 0.0f;
    float corner4 = 0.0f;
    
    int x_minus = x - step;
    int x_plus = x + step;
    int z_minus = z - step;
    int z_plus = z + step;
    
    if (vertices[(TOTAL * z) + x].y == INT_MIN) {
        if (x_minus >= 0) {
            corner1 = vertices[(TOTAL * z) + x_minus].y;
            counter++;
        }
        
        if (x_plus < TOTAL) {
            corner2 = vertices[(TOTAL * z) + x_plus].y;
            counter++;
        }
        
        if (z_minus >= 0) {
            corner3 = vertices[(TOTAL * z_minus) + x].y;
            counter++;
        }
        
        if (z_plus < TOTAL) {
            corner4 = vertices[(TOTAL * z_plus) + x].y;
            counter++;
        }
        
        if (corner1 == INT_MIN || corner2 == INT_MIN || corner3 == INT_MIN || corner4 == INT_MIN) {
            return;
        }
        
        float offset = (std::rand() % magnitude) + 1;
        float value = (offset * 0.1f) - (magnitude * 0.1f / 2.0f);
        vertices[(TOTAL * z) + x].y = ((corner1 + corner2 + corner3 + corner4) / counter) + (value);
    }
    
    int new_step = step / 2;
    
    x_minus = x - new_step;
    x_plus = x + new_step;
    z_minus = z - new_step;
    z_plus = z + new_step;
    
    if (magnitude - 2 != 0) {
        magnitude = magnitude - 2;
    }
    else {
        magnitude = 1;
    }
    
    if (new_step != 0) {
        if (x_minus >= 0) {
            if (z_minus >= 0) {
                diamond_step(x_minus, z_minus, new_step, magnitude);
            }
            
            if (z_plus < TOTAL) {
                diamond_step(x_minus, z_plus, new_step, magnitude);
            }
        }
        
        if (x_plus < TOTAL) {
            if (z_minus >= 0) {
                diamond_step(x_plus, z_minus, new_step, magnitude);
            }
            
            if (z_plus < TOTAL) {
                diamond_step(x_plus, z_plus, new_step, magnitude);
            }
        }
    }
}

void Terrain::draw(glm::mat4 C) {
    glUseProgram(shader);
    
    GLuint model_matrix = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(model_matrix, 1, GL_FALSE, &C[0][0]);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, ordered.size());
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Terrain::update() {
    seed = time(NULL);
    
    ordered.clear();
    normals.clear();
    
    this->setData();
}
//std::vector <glm::vec3> Terrain::returnVertices(int num, glm::vec3 translate, float scale) {
//
//    std::vector <glm::vec3> surfacePoints;
//    for (int i=0; i<num; i++){
//        surfacePoints.push_back((scale*ordered[i].x)+,);
//    }
//    return surfacePoints;
//};
