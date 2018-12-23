#version 330 core

uniform mat4 projection;
uniform mat4 modelview;


in vec3 fragNormal;
in vec3 fragVert;
out vec4 color;

void main()
{
    vec3 n = normalize(transpose(inverse(mat3(modelview))) * fragNormal);
    color = vec4(n * vec3(0.0f,0.0f,1.0f), 1.0f); //blue
}


