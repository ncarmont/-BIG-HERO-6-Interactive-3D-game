//
//  Transform.cpp
//  Hw3CSE167
//
//  Created by Computer Science on 09/11/2018.
//  Copyright © 2018 Nicolas Carmont. All rights reserved.
//
#include "Tree.hpp"
#include <string>

Tree::Tree(glm::mat4 C,glm::vec3 startPoint, std::string rule, int recursionNum , float leafScale, float initScale){
    
    CGL = C;
    startPointGL=startPoint;
    recursionNumGL =recursionNum;
    leafScaleGL = leafScale;
    initScaleGL = initScale;
    int seed = time(NULL);
    this->setData(rule, leafScale,initScale, seed);
    
};

Tree::~Tree(){
    
    unitDirVec.clear();
    lastPoint.clear();
    pointStorer.clear();
    dirStorer.clear();
    colorScaleVec.clear();
    branchVec.clear();
    for (int i = 0; i<branchVec.size(); i++){
        delete(branchVec[i]);
    };
};



// This is where you draw everything
void Tree::draw(GLuint shaderProgram){
    for (int i = 0; i<branchVec.size(); i++){
        float col =colorScaleVec[i];
        branchVec[i]->draw(shaderProgram, col);
    };
    
}


void Tree::drawForward(glm::vec3 startPoint, glm::vec3 unitDir, float leaveScale, float initScale){
    float randZ = -1.0f+(rand() % 10)/5.0f;
    glm::vec3 scaleToRoot = glm::vec3(unitDir.x*initScale,unitDir.y*initScale, (unitDir.z+randZ)*initScale);
    glm::vec3 nextPoint = startPoint + glm::vec3(scaleToRoot.x*leaveScale, scaleToRoot.y*leaveScale,scaleToRoot.z*leaveScale );
    Branch * branch;
    branch = new Branch(startPoint,nextPoint);
    branchVec.push_back(branch);
    lastPoint.push_back(nextPoint);
    };


void Tree::turnLeft(glm::vec3 unitDir){
    float angleRand = 1.0*(2+rand() % 30);
    float angle = glm::radians(angleRand);
    float randZ = 1.0f*(-1+(rand() % 10)/10.0f);
    glm::mat4 mat = glm::rotate(glm::mat4(1),angle,glm::vec3(0.0f, 0.0f, 1.0f));
//    std::cout<<randZ<<std::endl;
    glm::vec3 rotVec = mat * glm::vec4(unitDir, 1.0f);
    glm::vec3 unitRotVec = glm::normalize(rotVec);
    unitDirVec.push_back(unitRotVec);
};


void Tree::turnRight(glm::vec3 unitDir){
    float angleRand = -1.0f*(2+ rand() % 30);
//    std::cout<<angleRand<<std::endl;
    float angle = glm::radians(angleRand);
    glm::mat4 mat = glm::rotate(glm::mat4(1),angle,glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 rotVec = mat * glm::vec4(unitDir, 1.0f);
    glm::vec3 unitRotVec = glm::normalize(rotVec);
    unitDirVec.push_back(unitRotVec);
};


void Tree::storePoint(glm::vec3 startPoint, glm::vec3 unitDir){
    pointStorer.push_back(startPoint);
    dirStorer.push_back(unitDir);
};

void Tree::pushPoint(){
    int m = pointStorer.size();
    glm::vec3 storedPoint = pointStorer[m-1];
    
    int l = dirStorer.size();
    glm::vec3 storedDir = dirStorer[l-1];
    
    lastPoint.push_back(storedPoint);
    unitDirVec.push_back(storedDir);
    
    pointStorer.pop_back();
    dirStorer.pop_back();
};

void Tree::setData(std::string rule, float leafScale, float initScale, int seed){
    
    //Set the random number generator with the seed
    std::srand(seed);
    
    std::string expandedString = rule;
    int counter = 0;

    while(counter != recursionNumGL){
        std::string tempStr = "";
        for(char& c : expandedString) {
            if(c == 'B'){
                tempStr+=rule;
            } else {
                tempStr+= c;
            }
        };
        expandedString = tempStr;

        if(counter == (recursionNumGL-1) ){
            expandedString = "";
            for(char& c : tempStr) {
                if(c == 'B'){
                    expandedString+="F";
                } else {
                    expandedString+= c;
                }
            }
        };
        counter++;
    };


    glm::vec3 unitDir = glm::vec3(0.0f,initScale*3.0f,0.0f);
    lastPoint.push_back(startPointGL);
    unitDirVec.push_back(unitDir);
    divideSmallerLeaves = 1.0000f;
    glm::vec3 strtPoint;
    float colorScale = 1.0f;
    int recCount = 0;
    for(char& c : expandedString) {

        int n = lastPoint.size();
        strtPoint =lastPoint[n-1];

        int j = unitDirVec.size();
        unitDir =unitDirVec[j-1];

        if( c == 'F'){
            drawForward(strtPoint, unitDir, divideSmallerLeaves, initScale);
            colorScaleVec.push_back(colorScale);
        }
        else if( c == '-'){
            turnLeft(unitDir);

        }
        else if( c == '+'){
            turnRight(unitDir);

        }
        else if( c == '['){
            storePoint(strtPoint, unitDir);
            divideSmallerLeaves *= 1/(leafScale * (0.85+0.15*(rand() % 1)));
            recCount+=1;
        }
        else if( c == ']'){
            pushPoint();
            divideSmallerLeaves *= (leafScale * (0.85+0.15*(rand() % 1)));
            recCount-=1;
        }

        if(recCount>4){
            colorScale= 3.0f;
        }
        else if(recCount>2){
            colorScale= 2.0f;
        }
        else{
            colorScale= 1.0f;
        }
    }

}

void Tree::update(std::string rule, float leafScale, float initScale){
    unitDirVec.clear();
    lastPoint.clear();
    pointStorer.clear();
    dirStorer.clear();
    colorScaleVec.clear();
    branchVec.clear();
    int seed = time(NULL);
    this->setData(rule, leafScale, initScale, seed);
}
