#include "Patch.h"

/*
 For each control point, calculate its tangent.
 First, calculate the point in u-v coordinates;
 i.e. FINDING V: p0, p4, p8, p12 => len(p0 to p8) / len(p0 to p12) [p0-p4 + p4-p8 + p8+p12]
 Calculate lerp for both v and u; get derivatives
 Cross product the result you get from v and u; that is normal
 */

//original default vertices
Patch::Patch()
{
    segments = 20;
    toWorld = glm::mat4(1.0f);
    
    //col1
    glm::vec3 p0 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 p4 = glm::vec3(0.0, 1.0f, -3.0f);
    glm::vec3 p8 = glm::vec3(0.0f, -1.0f, -4.0f);
    glm::vec3 p12 = glm::vec3(0.0f, 2.0f, -7.0f);

    //col2
    glm::vec3 p1 = glm::vec3(3.0f, 1.0f, 0.0f);
    glm::vec3 p5 = glm::vec3(3.0f, 2.0f, -3.0f);
    glm::vec3 p9 = glm::vec3(3.0f, 1.0f, -4.0f);
    glm::vec3 p13 = glm::vec3(3.0f, 2.0f, -7.0f);

    //col3
    glm::vec3 p2 = glm::vec3(4.0f, -1.0f, 0.0f);
    glm::vec3 p6 = glm::vec3(4.0f, 0.0f, -3.0f);
    glm::vec3 p10 = glm::vec3(4.0f, -1.0f, -4.0f);
    glm::vec3 p14 = glm::vec3(4.0f, 1.0f, -7.0f);

    //col4
    glm::vec3 p3 = glm::vec3(7.0f, 0.0f, 0.0f);
    glm::vec3 p7 = glm::vec3(7.0f, 1.0f, -3.0f);
    glm::vec3 p11 = glm::vec3(7.0f, 0.0f, -4.0f);
    glm::vec3 p15 = glm::vec3(7.0f, 2.0f, -7.0f);

    
 
    patch[0][0] = p12;
    patch[0][1] = p13;
    patch[0][2] = p14;
    patch[0][3] = p15;

    patch[1][0] = p8;
    patch[1][1] = p9;
    patch[1][2] = p10;
    patch[1][3] = p11;

    patch[2][0] = p4;
    patch[2][1] = p5;
    patch[2][2] = p6;
    patch[2][3] = p7;

    patch[3][0] = p0;
    patch[3][1] = p1;
    patch[3][2] = p2;
    patch[3][3] = p3;

    
    populateVertNorms();
    populateIndices();
    setup();
}

Patch::Patch(glm::vec3 arr[4][4])
{
    
    toWorld = glm::mat4(1.0f);

    for (int i = 0; i<4; i++){
        for (int j = 0; j < 4; j++){
            patch[i][j] = arr[i][j];
        }
    }
    populateVertNorms();
    populateIndices();
    setup();
}


//clear all data, repopulate vertex norms and indices
void Patch::reset(){
    cp_i.clear();
    cp_n.clear();
    cp_v.clear();
    
    populateVertNorms();
    populateIndices();
    rebindBuffers();
}


void Patch::setup(){
    //set up
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBON);
    
    glBindVertexArray(VAO);
    
    //vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cp_v.size()*sizeof(glm::vec3), &cp_v[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cp_i.size() *sizeof(glm::vec3), &cp_i[0], GL_STATIC_DRAW);
    
    //normals
    glBindBuffer(GL_ARRAY_BUFFER, VBON);
    glBufferData(GL_ARRAY_BUFFER, cp_n.size() * sizeof(glm::vec3), &cp_n[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


//calculates the normal of a vertex
void Patch::populateVertNorms(){
    //for every v, calculate along u first.
    //calculate points along the curves
    //calculate normals of those points
    for (int i = 0; i < segments; i++) {
        float v = (float) i / (segments - 1);
        for (int j = 0; j < segments; j++) {
            float u = (float)j / (segments - 1);
            cp_v.push_back(calcPoint(u,v));
            cp_n.push_back(calcNormal(u,v));
        }
    }
}

//adds order of vertices to be connected (triangles)
void Patch::populateIndices(){
    
    //calculations of when the last triangle is, adds connecting indices order
    int last_triangle = ((segments-1)*(segments-1)+(segments-4+1));
    
    for (int i = 0; i <= last_triangle; i++){
        if ( ((i+1)%segments) == 0 ){ //new row
            continue;
        }
        cp_i.push_back(i);
        cp_i.push_back(i+1);
        cp_i.push_back(i+1+segments);
        cp_i.push_back(i+1+segments);
        cp_i.push_back(i+segments);
        cp_i.push_back(i);
    }
}


//generates all four anchor points on right plus continuity points
void Patch::regenerate_right(){

    //higher chance of anchor points moving [0][0], [0][3], [3][0], [3][3]
    patch[0][0].y = ((rand() % 3) - 1) / 2.0; //-.5 to .5
    patch[0][3].y = ((rand() % 3) - 1) / 2.0;
    patch[3][0].y = ((rand() % 3) - 1) / 2.0;
    patch[3][3].y = ((rand() % 3) - 1) / 2.0;

    //sets the random amount of other control points to a random amount (-.25,0,.25)
    int runs = rand() % 6 + 1;
    for (int i = 0; i < runs; i++){
        int x = rand() % 2 + 1; //gets index 1/2
        int y = rand() % 2 + 1;
        float toAdd = ((rand() % 3) - 1) / 10.0; //-.25 to .25
        patch[x][y].y = toAdd;
    }
}

//regenerates left patch -- doesn't touch right 2 anchor points
void Patch::regenerate_left(){
    
    //higher chance of anchor points moving [0][0], [3][0]
    patch[0][0].y = ((rand() % 3) - 1) / 2.0; //-.5 to .5
    patch[3][0].y = ((rand() % 3) - 1) / 2.0;
    
    //sets the random amount of other control points to a random amount (-.25,0,.25)
    int runs = rand() % 6 + 1;
    for (int i = 0; i < runs; i++){
        int x = rand() % 2 + 1; //gets index 1/2
        int y = rand() % 2 + 1;
        float toAdd = ((rand() % 3) - 1) / 10.0; //-.25 to .25
        patch[x][y].y = toAdd;
    }
}

//rebind buffers after change
void Patch::rebindBuffers(){
    
    //vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cp_v.size() * 3 * sizeof(GLfloat), &cp_v[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //indices
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cp_i.size() * 3 * sizeof(GLfloat), &cp_i[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //normals?? does this need regenerating
    glBindBuffer(GL_ARRAY_BUFFER, VBON);
    glBufferSubData(GL_ARRAY_BUFFER, 0, cp_n.size() * 3 * sizeof(GLfloat), &cp_n[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Patch::~Patch()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBON);
}

void Patch::draw(GLuint shaderProgram)
{
    glm::mat4 modelview = Window::V * this->toWorld;
    GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
    GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)cp_i.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
}

//calculates bernstein bezier equation
glm::vec3 Patch::Bez(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

    float t2 = t*t;
    float t3 = t2*t;
    
    float b0 = (t3 * -1) + (3*t2) - (3*t) + 1;
    float b1 = (t3 * 3) - (6*t2) + (3*t);
    float b2 = (t3 * -3) + (3*t2);
    float b3 = t3;
    
    glm::vec3 x = p0*b0 + p1*b1 + p2*b2 + p3*b3;
    return x;
}


//calculates first derivative of bernstein bezier equation
glm::vec3 Patch::BezD(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {

    float t2 = t*t;
    
    float b0 = -(3*t2)+(6*t)-(3);
    float b1 = (9*t2)-(12*t)+(3);
    float b2 = (-9*t2)+(6*t);
    float b3 = (3*t2);

    glm::vec3 x = p0*b0 + p1*b1 + p2*b2 + p3*b3;
    return x;
}


//calculate a point on a bezier curve
glm::vec3 Patch::calcPoint(float u, float v) {
    glm::vec3 q[4];
    
    q[0] = Bez(u, patch[3][0], patch[3][1], patch[3][2], patch[3][3]);
    q[1] = Bez(u, patch[2][0], patch[2][1], patch[2][2], patch[2][3]);
    q[2] = Bez(u, patch[1][0], patch[1][1], patch[1][2], patch[1][3]);
    q[3] = Bez(u, patch[0][0], patch[0][1], patch[0][2], patch[0][3]);
    
    glm::vec3 x = Bez(v, q[0], q[1], q[2], q[3]);
    return x;
}


//calculates the normal of a given point(u,v) on the patch
glm::vec3 Patch::calcNormal(float u, float v) {
    
    glm::vec3 q[4];
    q[0] = Bez(u, patch[3][0], patch[3][1], patch[3][2], patch[3][3]);
    q[1] = Bez(u, patch[2][0], patch[2][1], patch[2][2], patch[2][3]);
    q[2] = Bez(u, patch[1][0], patch[1][1], patch[1][2], patch[1][3]);
    q[3] = Bez(u, patch[0][0], patch[0][1], patch[0][2], patch[0][3]);

    glm::vec3 r[4];
    r[0] = Bez(v, patch[3][0], patch[2][0], patch[1][0], patch[0][0]);
    r[1] = Bez(v, patch[3][1], patch[2][1], patch[1][1], patch[0][1]);
    r[2] = Bez(v, patch[3][2], patch[2][2], patch[1][2], patch[0][2]);
    r[3] = Bez(v, patch[3][3], patch[2][3], patch[1][3], patch[0][3]);
   
    glm::vec3 x_dv = BezD(v, q[0], q[1], q[2], q[3]);
    glm::vec3 x_du = BezD(u, r[0], r[1], r[2], r[3]);

    // x_du X x_dv    cross product of tangents
    glm::vec3 normal;
    normal.x = (x_du.y * x_dv.z) - (x_du.z * x_dv.y);
    normal.y = (x_du.z * x_dv.x) - (x_du.x * x_dv.z);
    normal.z = (x_du.x * x_dv.y) - (x_du.y * x_dv.x);
    
    //normal = normalize(normal);
    return normal;
}


void Patch::scale(float s){
    glm::vec3 scaled = glm::vec3(s,s,s);
    glm::mat4 scaling = glm::scale(glm::mat4(), scaled);
    this->toWorld = scaling * this->toWorld;
}

void Patch::translate(float x, float y, float z){
    
    glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    this->toWorld = translationMat * this->toWorld;
    reset();
}

//moves the actual points instead of a translation matrix!
void Patch::move(float x, float y, float z){
    
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            patch[i][j] += glm::vec3(x,y,z);
        }
    }
    reset();
}
