#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in float greenColor;
//in vec3 FragPos;
//in vec3 Normal;
//
//uniform vec3 lightPos;
//uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
//glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


void main()
{
    //ambient
//    float ambientStrength = 0.1f;
//    vec3 ambient = ambientStrength * lightColor;
//
//    //diffuse
//    vec3 color1;
//    color1.x= 0.2f;
//    color1.y = 0.1f;
//    color1.z = 0.1f;
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    color = vec4(0.0f, greenColor, 0.1f, sampleExtraOutput);
    
}
