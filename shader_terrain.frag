#version 330 core
struct Light{
	vec3 d_light_color;
	vec3 d_light_pos;
};

in vec3 fragVertex;
in vec3 fragPosition;
in vec3 fragNormal;

out vec4 color;

uniform vec3 cameraPos;
uniform float shininess;
uniform Light light;

void main()
{

    if(fragPosition.y < 2.5){
        color = vec4(0.87, 0.65, 0.19, 1.0);
	}
	else{
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}

    vec3 d_ambient = light.d_light_color * vec3(color);

    //diffuse lighting
    vec3 d_norm = normalize(fragNormal);
    vec3 d_lightDir = normalize(-light.d_light_pos);

    float d_diff = dot(d_norm, d_lightDir);
    vec3 d_diffuse = d_diff * light.d_light_color * vec3(color);

    //specular lighting
    vec3 d_viewDir = normalize(cameraPos - fragVertex);
    vec3 d_reflectDir = reflect(-d_lightDir, d_norm);

    float d_specCom = pow(max(dot(d_viewDir, d_reflectDir), 0.0), shininess);
    vec3 d_specular = d_specCom * light.d_light_color * vec3(color);

    color = vec4((d_ambient + d_diffuse + d_specular), 1.0);

}
