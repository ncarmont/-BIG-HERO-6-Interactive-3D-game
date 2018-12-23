//
//  Transform.hpp
//  Hw3CSE167
//
//  Created by Computer Science on 09/11/2018.
//  Copyright © 2018 Nicolas Carmont. All rights reserved.
//

#ifdef _TREE_H_
#define _TREE_H_
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Branch.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>


class Tree
{
    public:
    Tree(glm::mat4 C,glm::vec3 startPoint, std::string rule, int recursionNum , float leafScale, float initScale);
    ~Tree();
    int totalRecursion;
    int stackCounter;
    int recCounter;
    float divideSmallerLeaves;
    
    glm::mat4 CGL;
    glm::vec3 startPointGL;
    std::string ruleGL;
    int recursionNumGL;
    float leafScaleGL;
    float initScaleGL;

    
    std::vector <glm::vec3> unitDirVec;
    std::vector <glm::vec3> lastPoint;
    std::vector <glm::vec3> pointStorer;
    std::vector <glm::vec3> dirStorer;
    std::vector <float> colorScaleVec;
    std::vector <Branch*> branchVec;
    
    
    
    void update(std::string rule, float leafScale, float initScale);
    void setData(std::string rule, float leafScale, float initScale, int seed);
    
    void turnLeft(glm::vec3 unitDir);
    void turnRight(glm::vec3 unitDir);
    void pushPoint();
    void drawForward(glm::vec3 startPoint, glm::vec3 unitDir, float leaveScale, float initScale);
    
    void draw(GLuint shaderProgram);
    void storePoint(glm::vec3 startPoint, glm::vec3 unitDir);
    };
