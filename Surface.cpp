#include "Surface.h"

/*
patch left sharing [0][3],[1][3],[2][3],[3][3]
patch right sharing [0][0],[1][0],[2][0],[3][0]

right cont  | left cont                 middle
right[0][1] | left[0][2] compared to right[0][0]
right[1][1] | left[1][2] compared to right[1][0]
right[2][1] | left[2][2] compared to right[2][0]
right[3][1] | left[3][2] compared to right[3][0]
*/


//surface is horizontal two patches
Surface::Surface(){
    
    //right patch will be original set up/points, share right's points
    right = new Patch();
    left = new Patch();
    left->move(-7.0f,0.0f,0.0f);
    
    //declarations
    glm::vec3 p1 = right->patch[0][0];
    glm::vec3 p2 = right->patch[1][0];
    glm::vec3 p3 = right->patch[2][0];
    glm::vec3 p4 = right->patch[3][0];
    
    //makes sure anchor points are same
    left->patch[0][3] = p1;
    left->patch[1][3] = p2;
    left->patch[2][3] = p3;
    left->patch[3][3] = p4;
    
    
    //move left patch points so c1 continuity
    left->patch[0][2] = p1-(right->patch[0][1] - p1);
    left->patch[1][2] = p2-(right->patch[1][1] - p2);
    left->patch[2][2] = p3-(right->patch[2][1] - p3);
    left->patch[3][2] = p4-(right->patch[3][1] - p4);
    
    //rebinds buffers
    left->reset();
}


void Surface::regenerate(){
    
    //regenerates right half
    right->regenerate_right();

    //declarations
    glm::vec3 p1 = right->patch[0][0];
    glm::vec3 p2 = right->patch[1][0];
    glm::vec3 p3 = right->patch[2][0];
    glm::vec3 p4 = right->patch[3][0];

    //makes sure anchor points are same
    left->patch[0][3] = p1;
    left->patch[1][3] = p2;
    left->patch[2][3] = p3;
    left->patch[3][3] = p4;

    //move left patch points so c1 continuity
    left->patch[0][2] = p1-(right->patch[0][1] - p1);
    left->patch[1][2] = p2-(right->patch[1][1] - p2);
    left->patch[2][2] = p3-(right->patch[2][1] - p3);
    left->patch[3][2] = p4-(right->patch[3][1] - p4);


    //regenerates left half
    left->regenerate_left();

    //declarations
     p1 = left->patch[0][3];
     p2 = left->patch[1][3];
     p3 = left->patch[2][3];
     p4 = left->patch[3][3];

    //move right patch points so c1 continuity
    right->patch[0][1] = p1 + (p1 - left->patch[0][2]);
    right->patch[1][1] = p2 + (p2 - left->patch[1][2]);
    right->patch[2][1] = p3 + (p3 - left->patch[2][2]);
    right->patch[3][1] = p4 + (p4 - left->patch[3][2]);
    

    //reset buffers
    left->reset();
    right->reset();
    
}


// pass on functions

void Surface::draw(GLuint shaderProgram){
    right->draw(shaderProgram);
    left->draw(shaderProgram);
}

void Surface::scale(float s){
    right->scale(s);
    left->scale(s);
}

void Surface::translate(float x, float y, float z){
    left->translate(x,y,z);
    right->translate(x,y,z);
}

//moves the actual points instead of a translation matrix!
void Surface::move(float x, float y, float z){
    left->move(x,y,z);
    right->move(x,y,z);
}
