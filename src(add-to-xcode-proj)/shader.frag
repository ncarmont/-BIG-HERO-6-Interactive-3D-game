#version 330 core

/*** left only used uncommented ***/

//uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 view;
//uniform samplerCube skybox;

//uniform sampler2D tex;
//in vec2 fragTexCoord;

in vec3 fragNormal; //normal
in vec3 fragVert; //position
out vec4 color;

//can use for lighting and camera calcs
//uniform vec3 cameraPos;
//uniform vec3 lightPos;
//uniform vec3 lightColor;


void main()
{
    //color everything hot pink color
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

//    //reflecting sphere
//    vec3 incident_eye = normalize(fragVert);
//    vec3 normals = normalize(fragNormal);
//    vec3 reflected = reflect(incident_eye, normals);
//    // convert from eye to world space
//    reflected = vec3(inverse(view) * vec4(reflected, 0.0));
//    color = texture(skybox, reflected);
    
//    //basic normal coloring
//    vec3 n = normalize(transpose(inverse(mat3(modelview))) * fragNormal);
//    color = vec4(n, 1.0f) * (1.0f);
    
    
}

