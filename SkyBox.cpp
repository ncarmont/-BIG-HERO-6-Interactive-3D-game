#include "SkyBox.h"
#include "Window.h"
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <math.h>   // include math functions, such as sin, cos, M_PI
using namespace std;



float size = 100.0f;

GLfloat skyboxVertices[] = {
    // Positions
    -size,  size, -size,
    -size, -size, -size,
    size, -size, -size,
    size, -size, -size,
    size,  size, -size,
    -size,  size, -size,
    
    -size, -size,  size,
    -size, -size, -size,
    -size,  size, -size,
    -size,  size, -size,
    -size,  size,  size,
    -size, -size,  size,
    
    size, -size, -size,
    size, -size,  size,
    size,  size,  size,
    size,  size,  size,
    size,  size, -size,
    size, -size, -size,
    
    -size, -size,  size,
    -size,  size,  size,
    size,  size,  size,
    size,  size,  size,
    size, -size,  size,
    -size, -size,  size,
    
    -size,  size, -size,
    size,  size, -size,
    size,  size,  size,
    size,  size,  size,
    -size,  size,  size,
    -size,  size, -size,
    
    -size, -size, -size,
    -size, -size,  size,
    size, -size, -size,
    size, -size, -size,
    -size, -size,  size,
    size, -size,  size
};

SkyBox::SkyBox()
{
 
    toWorld = glm::mat4(1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


    cubemapTexture = loadCubemap();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void SkyBox::draw(GLuint shaderProgram)
{
  
    glm::mat4 modelview = glm::mat4(glm::mat3(Window::V)) * toWorld;
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

    
    glBindVertexArray(VAO);
//
//    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0+0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
//    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//    glDrawElements(GL_TRIANGLES, (GLsizei)faces.size(), GL_UNSIGNED_INT, 0);
//    glDepthMask(GL_TRUE);
    glBindVertexArray(0);
    

}



SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
}



GLuint SkyBox::loadCubemap()
{
    GLuint texture[1];     // storage for one texture
    glGenTextures(1, &texture[0]); // Create ID for texture

    // Set this texture to be the one we are working with
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);
    glActiveTexture(GL_TEXTURE0+0);

    std::vector <std::string> textures_faces = //right, left, top, bottom, back, front
//    {"right.ppm","left.ppm","up.ppm","down.ppm","back.ppm","front.ppm"};
//    {"right.tga","left.tga","up.tga","down.tga","back.tga","front.tga"};
    {"right.JPG","left.JPG","up.JPG","down.JPG","back.JPG","front.JPG"};

    int width, height, nrComponents;
    unsigned char *data;
    for(GLuint i = 0; i < textures_faces.size(); i++)
    {
//        data = loadPPM(textures_faces[i].c_str(), width, height);
          data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrComponents, 0);

        
  
        glTexImage2D(
                     GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                     );
        stbi_image_free(data);
        
    }

    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture[0];
}






/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* SkyBox::loadPPM(const char* filename, int& width, int& height)
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
    read = (unsigned int)fread(rawData, (unsigned int)width * height * 3, 1, fp);
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
