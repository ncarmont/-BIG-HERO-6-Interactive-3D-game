#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


out vec3 fragVertex;
out vec3 fragPosition;
out vec3 fragNormal;

void main()
{
	fragPosition = position;

	mat4 modelview = view * model;

	fragVertex = vec3(modelview * vec4(position, 1.0));
	fragNormal = mat3(transpose(inverse(modelview))) * normals;

    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
}

