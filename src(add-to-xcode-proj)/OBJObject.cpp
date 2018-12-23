#include "OBJObject.h"
#include "Window.h"

#include <iostream>
#include <fstream>
#include <math.h>   // include math functions, such as sin, cos, M_PI
using namespace std;



OBJObject::OBJObject(const char *filepath) 
{

   // this->angle = 0.0f;
    parse(filepath);
    printf("PARSING FINISHED\n");
    center();
    
    toWorld = glm::mat4(1.0f);
    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBON); //?? norm
//    glGenBuffers(1, &VBOT);
    
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

    
//    //Texture
//    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
//    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
//
//    OBJObject::loadTexture();
   
    //Draws indices/Faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*sizeof(int), &faces[0], GL_STATIC_DRAW);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram)
{
    glm::mat4 modelview = Window::V * toWorld;
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    GLuint uModel = glGetUniformLocation(shaderProgram, "model");
    GLuint uView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
    
    
    glBindVertexArray(VAO);
    
   
    glDrawElements(GL_TRIANGLES, (GLsizei)faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}



OBJObject::~OBJObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBON);
}


void OBJObject::setModel(glm::mat4 model){
    this->toWorld = model;
}

//for complex items with textures and more face items, ie robot stuff
//void OBJObject::parse(const char *filepath)
//{
//    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
//
//    /* This next part taken from the write up, slightly edited */
//
//    FILE* file;     // file pointer
//    float vx,vy,vz;     // vertex coordinates
//    float vnx,vny,vnz;  // vertex color
//    float vtx, vty;     //texture coordinates
//    int tx,ty,tz; // texture indcies
//    int fx,fy,fz; // face indices
//    int nx,ny,nz; // normal indices
//
//    int c1,c2;    // characters read from file
//
//
//    std::vector<glm::vec2> input_textures;
//    std::vector<glm::vec3> input_vertices;
//    std::vector<glm::vec3> input_normals;
//    std::vector<int> input_indices;
//
//    file = fopen(filepath,"rb");  // make the file name configurable so you can load other files
//    if (file == NULL) {
//        std::cerr << "error loading file" << std::endl;
//        exit(-1);
//    }  // just in case the file can't be found or is corrupt
//
//
//    do{
//        c1 = fgetc(file);
//        c2 = fgetc(file);
//
//        //printf("\nProcess %c%c", c1, c2);
//
//        //v
//        if ((c1=='v') && (c2==' ')){
//            fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
//            input_vertices.push_back({vx,vy,vz});
//            //printf("v %f %f %f\n", vx, vy, vz);
//        }
//        //vn
//        else if ((c1=='v') && (c2=='n')){
//            fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
//            input_normals.push_back({vnx, vny, vnz});
//        }
//        //vt
//        else if ((c1=='v') && (c2=='t')){
//            fscanf(file, "%f %f\n", &vtx, &vty);
//            input_textures.push_back({vtx, vty});
//            //printf("vt %f %f\n", vtx, vty);
//        }
//        else if ((c1 == 'f') && (c2 == ' ' )){
//            //second throw away values?? check to see if theyre identical
//            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",&fx,&tx,&nx,&fy,&ty,&ny,&fz,&tz,&nz);
//            if (fx == -1 || fy == -1 || fz == -1){ //for error catching purposes
//                printf("\n\n\n PARSING ISSUE \n\n\n");
//                break;
//            }
//            input_indices.push_back(fx-1);
//            input_indices.push_back(fy-1);
//            input_indices.push_back(fz-1);
//            t_indices.push_back(tx-1);
//            t_indices.push_back(ty-1);
//            t_indices.push_back(tz-1);
//            n_indices.push_back(nx-1);
//            n_indices.push_back(ny-1);
//            n_indices.push_back(nz-1);
//
//            fx=-1; fy=-1; fz=-1; //for testing purposes
//        }
//        else if(c2=='v'){
//            int c3=fgetc(file);
//           // printf("->%c",c3 );
//
//            //if ((c2=='v') && (fgetc(file)==' ')){
//            if ((c2=='v') && (c3==' ')){
//                fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
//                input_vertices.push_back({vx,vy,vz});
//            }else if((c2=='v') && (c3=='t')){
//                fscanf(file, "%f %f\n", &vtx, &vty);
//                input_textures.push_back({vtx, vty});
//            }else if((c2=='v') && (c3=='n')){
//                fscanf(file, "%f %f %f\n", &vnx, &vny, &vnz);
//                input_normals.push_back({vnx, vny, vnz});
//            }
//        }else if ((c2 == 'f') && (fgetc(file)==' ')){ //first f
//            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",&fx,&tx,&nx,&fy,&ty,&ny,&fz,&tz,&nz);
//            input_indices.push_back(fx-1);
//            input_indices.push_back(fy-1);
//            input_indices.push_back(fz-1);
//            t_indices.push_back(tx-1);
//            t_indices.push_back(ty-1);
//            t_indices.push_back(tz-1);
//            n_indices.push_back(nx-1);
//            n_indices.push_back(ny-1);
//            n_indices.push_back(nz-1);
//        }
////        else if ((c1=='v') && (c2=='n') && (fgetc(file)==' ')){
////            fscanf(file, "%f %f %f\n", &x, &y, &z);
////            normals.push_back({x,y,z});
////        }
//
//
//    }while(c1 != EOF && c2 != EOF);
//
//    fclose(file);   // make sure you don't forget to close the file when done
//
////    printf("\n\n\nvertices: %d\n", vertices.size());
////    printf("normals: %d\n", normals.size());
////    printf("textures: %d\n", textures.size());
////    printf("indices: %d\n", faces.size());
////    printf("t_indices: %d\n", t_indices.size());
////    printf("n_indices: %d\n\n\n\n", n_indices.size());
//
//
//    for (unsigned i = 0; i < input_indices.size(); i++) {
//        vertices.push_back(input_vertices[input_indices[i]]);
//        normals.push_back(input_normals[n_indices[i]]);
//        textures.push_back(input_textures[t_indices[i]]);
//        faces.push_back(i);
//    }
//
//
//
//}


//for eyeball sphere
//void OBJObject::parse(const char *filepath)
//{
//    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
//
//    /* This next part taken from the write up, slightly edited */
//
//    FILE* file;     // file pointer
//    float x,y,z;  // vertex coordinates
//    float r,g,b;  // vertex color
//    int fx,fy,fz; // face indices
//    int c1,c2;    // characters read from file
//
//
//
//    file = fopen(filepath,"rb");  // make the file name configurable so you can load other files
//    if (file == NULL) {
//        std::cerr << "error loading file" << std::endl;
//        exit(-1);
//    }  // just in case the file can't be found or is corrupt
//
//
//    do{
//        c1 = fgetc(file);
//        c2 = fgetc(file);
//
//        if ((c1=='v') && (c2==' ')){
//            fscanf(file, "%f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b);
//            vertices.push_back({x,y,z});
//        }
//        else if ((c1 == 'f') && (c2 == ' ' )){
//            //second throw away values?? check to see if theyre identical
//            fscanf(file, "%d//%d %d//%d %d//%d\n", &fx, &fx, &fy, &fy, &fz, &fz);
//            if (fx == -1 && fy == -1 && fz == -1){
//                break;
//            }
//            faces.push_back(fx-1);
//            faces.push_back(fy-1);
//            faces.push_back(fz-1);
//        }
//        else if ((c2 == 'f') && (fgetc(file)==' ')){ //first f
//            //second throw away values?? check to see if theyre identical
//            fscanf(file, "%d//%d %d//%d %d//%d\n", &fx, &fx, &fy, &fy, &fz, &fz);
//            faces.push_back(fx-1);
//            faces.push_back(fy-1);
//            faces.push_back(fz-1);
//        }
//        else if ((c1=='v') && (c2=='n') && (fgetc(file)==' ')){
//            fscanf(file, "%f %f %f\n", &x, &y, &z);
//            normals.push_back({x,y,z});
//        }
//
//
//    }while(c1 != EOF && c2 != EOF);
//
//    fclose(file);   // make sure you don't forget to close the file when done
//
//    std::cout << "Vertices: " << vertices.size() << std::endl;
//    std::cout << "Normals: " << normals.size() << std::endl;
//    std::cout << "Faces: " << faces.size() << std::endl;
//
//    printf("First vertice: {%f,%f,%f}\n", vertices[0][0],vertices[0][1],vertices[0][2]);
//    printf("First normal: {%f,%f,%f}\n", normals[0][0],normals[0][1],normals[0][2]);
//    int s = faces.size();
//    printf("First face: {%d,%d,%d}\n", faces[s-3],faces[s-2],faces[s-1]);
//}


//sphere, bunny, etc
void OBJObject::parse(const char *filepath)
{
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data

    /* This next part taken from the write up, slightly edited */

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



void OBJObject::center(){
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

void OBJObject::update()
{
    //move();
}

void OBJObject::move(){
    
    
}

glm::vec3 OBJObject::getCenter(){
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
    
    return glm::vec3(mid_x, mid_y, mid_z);
    
}


void OBJObject::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::scale(float s){
    glm::vec3 scaled = glm::vec3(s,s,s);
    glm::mat4 scaling = glm::scale(glm::mat4(), scaled);
    this->toWorld = scaling * this->toWorld;
}


//void OBJObject::rotate(float angle, glm::vec3 axis){
//    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle / 180.0f * glm::pi<float>(), axis);
//    this->toWorld = matrix * this->toWorld;
//}

void OBJObject::translate(float x, float y, float z)
{
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    this->toWorld = translationMat * this->toWorld;
}

void OBJObject::translate(glm::vec3 v)
{
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(v.x, v.y, v.z));
    this->toWorld = translationMat * this->toWorld;
}





//////////////

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* OBJObject::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    
    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    
    return rawData;
}

// load image file into texture object
void OBJObject::loadTexture()
{
    GLuint texture[1];     // storage for one texture
    int twidth, theight;   // texture width/height [pixels]
    unsigned char* tdata;  // texture pixel data
    
    // Load image file
    tdata = loadPPM("mochi.PPM", twidth, theight);
    if (tdata==NULL) return;
    
    // Create ID for texture
    glGenTextures(1, &texture[0]);
    
    // Set this texture to be the one we are working with
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE0+0);
    
    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
    
    // Set bi-linear filtering for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
