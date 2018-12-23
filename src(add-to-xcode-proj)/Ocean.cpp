#include "Ocean.h"

//connecting two surfaces, one top one bottom

Ocean::Ocean(){
    
    //top will be original set up/points, share top's points
    top = new Surface();
    bottom = new Surface();
    bottom->move(0.0f,0.0f,7.0f);
    
    align();

    //rebinds buffers
    bottom->left->reset();
    bottom->right->reset();
}

//aligns for c1 continuity of top and bottom surfaces
void Ocean::align(){
    
    //declarations of anchor points top row
    glm::vec3 p1 = top->left->patch[3][0];
    glm::vec3 p2 = top->left->patch[3][1];
    glm::vec3 p3 = top->left->patch[3][2];
    glm::vec3 p4 = top->left->patch[3][3];
    glm::vec3 p5 = top->right->patch[3][0];
    glm::vec3 p6 = top->right->patch[3][1];
    glm::vec3 p7 = top->right->patch[3][2];
    glm::vec3 p8 = top->right->patch[3][3];
    
    //makes sure anchor points are same
    bottom->left->patch[0][0] = p1;
    bottom->left->patch[0][1] = p2;
    bottom->left->patch[0][2] = p3;
    bottom->left->patch[0][3] = p4;
    bottom->right->patch[0][0] = p5;
    bottom->right->patch[0][1] = p6;
    bottom->right->patch[0][2] = p7;
    bottom->right->patch[0][3] = p8;
    
    //move bottom patch points so c1 continuity
    bottom->left->patch[1][0] = p1-(top->left->patch[2][0] - p1);
    bottom->left->patch[1][1] = p2-(top->left->patch[2][1] - p2);
    bottom->left->patch[1][2] = p3-(top->left->patch[2][2] - p3);
    bottom->left->patch[1][3] = p4-(top->left->patch[2][3] - p4);
    bottom->right->patch[1][0] = p5-(top->right->patch[2][0] - p5);
    bottom->right->patch[1][1] = p6-(top->right->patch[2][1] - p6);
    bottom->right->patch[1][2] = p7-(top->right->patch[2][2] - p7);
    bottom->right->patch[1][3] = p8-(top->right->patch[2][3] - p8);
}

void Ocean::regenerate(){
    top->regenerate();
    bottom->regenerate();
    align();
    top->right->reset();
    top->left->reset();
    bottom->right->reset();
    bottom->left->reset();
    
}

//passing functions to small surfaces
void Ocean::draw(GLuint shaderProgram){
    top->draw(shaderProgram);
    bottom->draw(shaderProgram);
}

void Ocean::scale(float s){
    top->scale(s);
    bottom->scale(s);
}

//moves the actual points instead of a translation matrix!
void Ocean::translate(float x, float y, float z){
    top->translate(x,y,z);
    bottom->translate(x,y,z);
}

glm::vec3 Ocean::getCenter(){
    return top->right->patch[3][0];
}
