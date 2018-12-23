#include "Mochi.h"
#include "Window.h"

#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <math.h>   // include math functions, such as sin, cos, M_PI
using namespace std;



Mochi::Mochi(const char *filepath)
{
    
    toWorld = glm::mat4(1.0f);
    parse(filepath);
    center();
    printf("Mochi Finished Parsing\n");
    lastTranslation = glm::vec3(0.0f);
    
    //setup
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBON);
    glGenBuffers(1, &VBOT);
    
    glBindVertexArray(VAO);
    
    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
  
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER, VBON);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    
    //Texture
    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    Mochi::loadTexture();
   
    //Draws indices/Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(int), &faces[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mochi::draw(GLuint shaderProgram)
{
    glm::mat4 modelview = Window::V * toWorld;
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
    
    glBindVertexArray(VAO);
   
    glDrawElements(GL_TRIANGLES, (GLsizei)faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}


Mochi::~Mochi()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBON);
    glDeleteBuffers(1, &VBOT);
}



void Mochi::parse(const char *filepath)
{
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* file;     // file pointer
    float vx,vy,vz;     // vertex coordinates
    float vnx,vny,vnz;  // vertex color
    float vtx, vty;     //texture coordinates
    int tx,ty,tz; // texture indcies
    int fx,fy,fz; // face indices
    int nx,ny,nz; // normal indices

    int c1,c2;    // characters read from file


    std::vector<glm::vec2> input_textures;
    std::vector<glm::vec3> input_vertices;
    std::vector<glm::vec3> input_normals;
    std::vector<int> input_indices;

    file = fopen(filepath,"rb");  // make the file name configurable so you can load other files
    if (file == NULL) {
        std::cerr << "error loading file mochi" << std::endl;
        exit(-1);
    }  // just in case the file can't be found or is corrupt


    do{
        c1 = fgetc(file);
        c2 = fgetc(file);

        //printf("\nProcess %c%c", c1, c2);

        //v
        if ((c1=='v') && (c2==' ')){
            fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
            input_vertices.push_back({vx,vy,vz});
            //printf("v %f %f %f\n", vx, vy, vz);
        }
        //vn
        else if ((c1=='v') && (c2=='n')){
            fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
            input_normals.push_back({vnx, vny, vnz});
        }
        //vt
        else if ((c1=='v') && (c2=='t')){
            fscanf(file, "%f %f\n", &vtx, &vty);
            input_textures.push_back({vtx, vty});
            //printf("vt %f %f\n", vtx, vty);
        }
        else if ((c1 == 'f') && (c2 == ' ' )){
            //second throw away values?? check to see if theyre identical
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",&fx,&tx,&nx,&fy,&ty,&ny,&fz,&tz,&nz);
            if (fx == -1 || fy == -1 || fz == -1){ //for error catching purposes
                printf("\n\n\n PARSING ISSUE \n\n\n");
                break;
            }
            input_indices.push_back(fx-1);
            input_indices.push_back(fy-1);
            input_indices.push_back(fz-1);
            t_indices.push_back(tx-1);
            t_indices.push_back(ty-1);
            t_indices.push_back(tz-1);
            n_indices.push_back(nx-1);
            n_indices.push_back(ny-1);
            n_indices.push_back(nz-1);

            fx=-1; fy=-1; fz=-1; //for testing purposes
        }
        else if(c2=='v'){
            int c3=fgetc(file);
           // printf("->%c",c3 );

            //if ((c2=='v') && (fgetc(file)==' ')){
            if ((c2=='v') && (c3==' ')){
                fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
                input_vertices.push_back({vx,vy,vz});
            }else if((c2=='v') && (c3=='t')){
                fscanf(file, "%f %f\n", &vtx, &vty);
                input_textures.push_back({vtx, vty});
            }else if((c2=='v') && (c3=='n')){
                fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
                input_normals.push_back({vnx, vny, vnz});
            }
        }else if ((c2 == 'f') && (fgetc(file)==' ')){ //first f
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",&fx,&tx,&nx,&fy,&ty,&ny,&fz,&tz,&nz);
            input_indices.push_back(fx-1);
            input_indices.push_back(fy-1);
            input_indices.push_back(fz-1);
            t_indices.push_back(tx-1);
            t_indices.push_back(ty-1);
            t_indices.push_back(tz-1);
            n_indices.push_back(nx-1);
            n_indices.push_back(ny-1);
            n_indices.push_back(nz-1);
        }

    }while(c1 != EOF && c2 != EOF);

    fclose(file);   // make sure you don't forget to close the file when done

    
    for (unsigned i = 0; i < input_indices.size(); i++) {
        vertices.push_back(input_vertices[input_indices[i]]);
        normals.push_back(input_normals[n_indices[i]]);
        textures.push_back(input_textures[t_indices[i]]);
        faces.push_back(i);
    }
}


//center object at 0,0,0
void Mochi::center(){
    float min_x = vertices[0].x, max_x = vertices[0].x;
    float min_y = vertices[0].y, max_y = vertices[0].y;
    float min_z = vertices[0].z, max_z = vertices[0].z;
    
    //find max and min of each dimension
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].x < min_x){
            min_x = vertices[i].x;
        }else if (vertices[i].x > max_x){
            max_x = vertices[i].x;
        }
        
        if (vertices[i].y < min_y){
            min_y = vertices[i].y;
        }else if (vertices[i].y > max_y){
            max_y = vertices[i].y;
        }
        
        if (vertices[i].z < min_z){
            min_z = vertices[i].z;
        }else if (vertices[i].z > max_z){
            max_z = vertices[i].z;
        }
    }
    
    //find midpoint
    float mid_x = (min_x + max_x) / 2;
    float mid_y = (min_y + max_y) / 2;
    float mid_z = (min_z + max_z) / 2;

    
    
    //center
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {   //center
        vertices[i].x -= mid_x;
        vertices[i].y -= mid_y;
        vertices[i].z -= mid_z;
    }
}


//gets the furthest vertex from the center, returns distance (used as bs radius)
int Mochi::getFurthest(){
    int max = 0;
    int check = 0;
    for (int i = 0; i < vertices.size(); i++){
        check = glm::distance(vertices[i],glm::vec3(0.0f,0.0f,0.0f));
        if (check > max){
            max = check;
        }
    }
    return max;
}


void Mochi::scale(float s){
    glm::vec3 scaled = glm::vec3(s,s,s);
    glm::mat4 scaling = glm::scale(glm::mat4(), scaled);
    this->toWorld = scaling * this->toWorld;
}

void Mochi::translate(float x, float y, float z)
{
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    this->toWorld = translationMat * this->toWorld;
//    lastTranslation = glm::vec3(x,y,z);
}


glm::mat4 Mochi::getModel(){
    return this->toWorld;
}

void Mochi::reset(){
    this->toWorld = glm::mat4(1.0f);
}


void Mochi::randomize(){
    std::srand(time(NULL));
    float x = (rand() % 11) * 1.0;
    float z = (rand() % 11) * 1.0;
    
    translate(x, -0.5f, z);
    
}

// load image file into texture object
void Mochi::loadTexture()
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load("mochi.JPEG", &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << std::endl;
        stbi_image_free(data);
    }
    
    //    return textureID;
}


