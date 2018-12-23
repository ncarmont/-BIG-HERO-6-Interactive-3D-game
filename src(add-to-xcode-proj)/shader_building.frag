#version 330 core
struct Light{
	vec3 d_light_color;
	vec3 d_light_pos;
};

in vec3 fragVertex;
in vec3 fragNormal;

out vec4 color;

uniform vec3 cameraPos;
uniform float shininess;
uniform Light light;
uniform int color_mode;

void main()
{
	//Version 1 coloring
	if(color_mode == 1){
		color = vec4(0.14, 0.0, 0.0, 1.0);
	}

	//Version 2 coloring
	else if(color_mode == 2){
		color = vec4(0.4, 0.5, 0.5, 1.0);
	}

	//Version 3 level 1 coloring
	else if(color_mode == 3){
		color = vec4(0.9, 0.75, 0.2, 1.0);
	}

	//Version 3 level 2 coloring
	else if(color_mode == 4){
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}

	//Version 3 roof coloring
	else if(color_mode == 5){
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}

	//Version 3 window coloring
	else{
		color = vec4(0.0, 0.6, 0.8, 1.0);
	}

	vec3 d_ambient = light.d_light_color * vec3(color);

	//Directional Lighting
	vec3 d_norm = normalize(fragNormal);
	vec3 d_lightDir = normalize(-light.d_light_pos);
	
	float d_diff = dot(d_norm, d_lightDir);
	vec3 d_diffuse = d_diff * light.d_light_color * vec3(color);

	vec3 d_viewDir = normalize(cameraPos - fragVertex);
	vec3 d_reflectDir = reflect(-d_lightDir, d_norm);  

	float d_specCom = pow(max(dot(d_viewDir, d_reflectDir), 0.0), shininess);
	vec3 d_specular = d_specCom * light.d_light_color * vec3(color);  

	color = vec4((d_ambient + d_diffuse + d_specular), 1.0);

}