//
//  Transform.hpp
//  Hw3CSE167
//
//  Created by Computer Science on 09/11/2018.
//  Copyright © 2018 Nicolas Carmont. All rights reserved.
//

#ifndef _FOREST_H_
#define _FOREST_H_

#include "Branch.h"
#include "Tree.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>


#endif /* Forest_hpp */

class Forest
{
    public:
    
    glm::mat4 CGL;
    glm::vec3 startPointGL;
    std::string ruleGL;
    int recursionNumGL;
    float leafScaleGL;
    float initScaleGL;
    std::vector <glm::vec3> terrainPosGL;
    
    std::vector <Tree*> treeVec;
    GLuint shader;
    
    Forest(glm::mat4 C,glm::vec3 startPoint, std::string rule, int recursionNum , float leafScale, float initScale, const std::vector <glm::vec3> &terrainPos);
    void setData(std::string rule);
    ~Forest();
    void draw(GLuint shaderProgram);
    void update(std::string rule, float leafScale, float initScale);
   
    
    
    
    };
