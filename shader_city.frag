#version 330 core

out vec4 color;

in vec3 fragVertex;

uniform int coloring;

void main()
{
	//Park coloring
	if(coloring == 1){
		color = vec4(0.0f, 0.5f, 0.0f, 1.0f);
	}
	
	//Street/Block coloring
	else{
		color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	}
}
