#version 330 core

layout (location = 0) in vec3 position;

out vec3 fragVertex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	mat4 modelview = view * model;

	fragVertex = position;

    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
}