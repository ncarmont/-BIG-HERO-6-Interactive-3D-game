#include "BoundingSphere.h"
#include "Window.h"

#include <iostream>
#include <fstream>
#include <math.h>   // include math functions, such as sin, cos, M_PI
using namespace std;

//initially enlarge due to largest radius from center of baymax
BoundingSphere::BoundingSphere(int r)
{
    parse("sphere.obj");
    center();   //centers the sphere since objects are centered
    enlarge(r); //increases the size of sphere by the radius
    radius = r; //sets radius
    lineColor = glm::vec3(0.0f,0.0f,0.0f); //default line color is black
    toWorld = glm::mat4(1.0f); //default is identity but gets updated w obj's toworld

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
  
    //Draws indices/Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(int), &faces[0], GL_STATIC_DRAW);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BoundingSphere::draw(GLuint shaderProgram)
{
    glUniform3fv(glGetUniformLocation(shaderProgram, "newcolor"),1, &lineColor[0] );
    glm::mat4 modelview = Window::V * toWorld;
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelview"), 1, GL_FALSE, &modelview[0][0]);
    
 
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
    
    
    glBindVertexArray(VAO);
    
   
    glDrawElements(GL_LINES, (GLsizei)faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}



BoundingSphere::~BoundingSphere()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


//scales the border of sphere
void BoundingSphere::enlarge(float s){
    for (int i = 0; i < vertices.size(); i++){
        vertices[i] = vertices[i] * s;
    }
}

//center the sphere
void BoundingSphere::center(){
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

void BoundingSphere::parse(const char* filepath)
{
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    
    FILE* file;     // file pointer
    float vx,vy,vz;     // vertex coordinates
    float vnx,vny,vnz;  // vertex color
    int fx,fy,fz; // face indices
    
    int c1,c2;    // characters read from file
    
    std::vector<glm::vec3> input_vertices;
    std::vector<glm::vec3> input_normals;
    std::vector<int> input_indices;
    
    file = fopen(filepath,"rb");  // make the file name configurable so you can load other files
    if (file == NULL) {
        std::cerr << "error loading file" << std::endl;
        exit(-1);
    }  // just in case the file can't be found or is corrupt
    
    
    do{
        c1 = fgetc(file);
        c2 = fgetc(file);
        
        //v
        if ((c1=='v') && (c2==' ')){
            fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
            input_vertices.push_back({vx,vy,vz});
        }
        //vn
        else if ((c1=='v') && (c2=='n')){
            fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
            input_normals.push_back({vnx, vny, vnz});
        }
        else if ((c1 == 'f') && (c2 == ' ' )){
            //second throw away values?? check to see if theyre identical
            fscanf(file, "%d//%d %d//%d %d//%d\n", &fx, &fx, &fy, &fy, &fz, &fz);
            if (fx == -1 || fy == -1 || fz == -1){ //for error catching purposes
                printf("\n\n\n PARSING ISSUE \n\n\n");
                break;
            }
            input_indices.push_back(fx-1);
            input_indices.push_back(fy-1);
            input_indices.push_back(fz-1);
            
            fx=-1; fy=-1; fz=-1; //for testing purposes
        }
        else if(c2=='v'){
            int c3=fgetc(file);
            // printf("->%c",c3 );
            
            //if ((c2=='v') && (fgetc(file)==' ')){
            if ((c2=='v') && (c3==' ')){
                fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
                input_vertices.push_back({vx,vy,vz});
            }else if((c2=='v') && (c3=='n')){
                fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
                input_normals.push_back({vnx, vny, vnz});
            }
        }else if ((c2 == 'f') && (fgetc(file)==' ')){ //first f
            fscanf(file, "%d//%d %d//%d %d//%d\n", &fx, &fx, &fy, &fy, &fz, &fz);
            input_indices.push_back(fx-1);
            input_indices.push_back(fy-1);
            input_indices.push_back(fz-1);
        }
        
        
    }while(c1 != EOF && c2 != EOF);
    
    fclose(file);   // make sure you don't forget to close the file when done
    
    unsigned i = 0;
    for ( ; i < input_vertices.size(); i++) {
        vertices.push_back(input_vertices[i]);
        normals.push_back(input_normals[i]);
        faces.push_back(input_indices[i]);
    }
    
    for (; i < input_indices.size(); i++) {
        faces.push_back(input_indices[i]);
    }
}


//checks if the two bounding spheres intersect, if yes return true, else false
bool BoundingSphere::checkIntersect(BoundingSphere* check){
    float currRadius = getRadius();
    float otherRadius = check->getRadius();

    glm::vec3 currCenter = getModel() * glm::vec4(getCenter(),1.0f);
    glm::vec3 otherCenter = check->getModel() * glm::vec4(getCenter(),1.0f);

    float currDist = glm::distance(currCenter, otherCenter);
    float totalDist = currRadius + otherRadius;
    return (currDist < totalDist);
}



void BoundingSphere::translate(float x, float y, float z)
{
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    this->toWorld = translationMat * this->toWorld;
}

void BoundingSphere::scale(float s){
    //moves radius during scale only
    radius *= s;
}

/*** getters and setters ***/

float BoundingSphere::getRadius(){
    return radius;
}

glm::vec3 BoundingSphere::getCenter(){
    return centerPoint;
}

glm::mat4 BoundingSphere::getModel(){
    return this->toWorld;
}

void BoundingSphere::setModel(glm::mat4 world){
    toWorld = world;
}

void  BoundingSphere::setColor(glm::vec3 v){
    //bounding sphere draw color
    lineColor = v;
}

