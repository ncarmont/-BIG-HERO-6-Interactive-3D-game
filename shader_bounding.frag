#version 330 core

uniform vec3 newcolor;
out vec4 color;


void main()
{
    //red if intersecting, black otherwise
    color = vec4(newcolor, 1.0f);
}

