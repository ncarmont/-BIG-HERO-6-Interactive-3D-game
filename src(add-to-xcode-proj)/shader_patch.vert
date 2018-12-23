#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 modelview;

out vec3 fragVert;
out vec3 fragNormal;

void main()
{
    gl_Position =  projection * modelview  * vec4(position.x, position.y, position.z, 1.0);
    
    fragNormal = normal;
    fragVert = position;
}


