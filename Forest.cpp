//
//  Transform.cpp
//  Hw3CSE167
//
//  Created by Computer Science on 09/11/2018.
//  Copyright © 2018 Nicolas Carmont. All rights reserved.
//
#include "Forest.hpp"
#include <string>


Forest::Forest(glm::mat4 C,glm::vec3 startPoint, std::string rule, int recursionNum , float leafScale, float initScale, const std::vector <glm::vec3> &terrainPos){
    
    
    
    CGL = C;
    startPointGL =startPoint;
    ruleGL = rule;
    recursionNumGL = recursionNum;
    leafScaleGL = leafScale;
    initScaleGL = initScale;
    terrainPosGL = terrainPos;
    this->setData(rule);
    // To render single tree:
    
//    Tree * tree;
//    tree = new Tree( C, startPoint,  rule,  recursionNum , leafScale);
//    treeVec.push_back(tree);

};

Forest::~Forest(){
    
    for (int i = 0; i<treeVec.size(); i++){
        delete(treeVec[i]);
    };
    
    treeVec.clear();
    
};

void Forest::draw(GLuint shaderProgram){
    
    shader =shaderProgram;
    for (int i =0; i<treeVec.size(); i++){
        treeVec[i]->draw(shader);
    };
    
}
    
void Forest::update(std::string rule, float leafScale, float initScale){
    
    for (int i =0; i<treeVec.size(); i++){
        treeVec[i]->update(rule, leafScale, initScale);
    };
//
    // To render single tree:
//    treeVec[0]->draw(shaderProgram);
};


void Forest::setData(std::string rule){
// To render multiple trees:
for (int i = 0; i<terrainPosGL.size(); i++){
    Tree * tree;
    glm::vec3 pos = terrainPosGL[i];
    tree = new Tree( CGL, pos,  rule,  recursionNumGL , leafScaleGL, initScaleGL);
    treeVec.push_back(tree);
};
}
