#version 330 core

layout (location = 0) in vec3 position; //vertices array bound
layout (location = 1) in vec3 normal;   //normal array bound
//layout (location = 2) in vec2 tex;


uniform mat4 projection;
uniform mat4 modelview;
//uniform mat4 model;
//uniform mat4 view;


out vec3 fragVert;//position
out vec3 fragNormal; //normal
//out vec2 fragTexCoord;


void main()
{
    
    gl_Position =  projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    
//    //pass to fragment shader
//    fragNormal = normal;
//    fragVert = position;
//    fragTexCoord = tex;
//
//    fragNormal = mat3(transpose(inverse(model))) * normal;
//    fragVert=vec3(model * vec4(position, 1.0));

    fragVert = vec3(modelview * vec4(position, 1.0));
    fragNormal = vec3(modelview * vec4(normal, 0.0));
}
