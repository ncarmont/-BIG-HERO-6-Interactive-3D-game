#version 330 core

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;
uniform mat4 view;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;


//in vec2 fragTexCoord;
in vec3 fragNormal; //normal
in vec3 fragVert; //position
out vec4 color;


void main()
{
    color = vec4(1.0f,1.0f,1.0f,1.0f);
    
    //normal in world coordinates
    vec3 normal = normalize(transpose(inverse(mat3(modelview))) * fragNormal);

    vec3 lighting = vec3(modelview * vec4(lightPos, 1.0f));
    vec3 cam = vec3(modelview * vec4(cameraPos, 1.0f));
    vec3 vert = vec3(modelview * vec4(fragVert, 1.0f));


    //silhouette edges and discretized shading
    vec3 incident_eye = normalize(vert);//-?
    vec3 n = normalize(normal);         //normal if i want camera lighting
    vec3 v = normalize(cam - vert);     //viewing direction
    vec3 l = normalize(lightPos - vert);//light direction
    
    float edge = max(0.0f,dot(n,v));
    float diffuse = max(0.0f,dot(n,l));
    

        if (diffuse > 0.75) {
            color = color * 1.0;
        }
        else if (diffuse > 0.50) {
            color = color * 0.8;
        }
        else if (diffuse > 0.25) {
            color = color * 0.6;
        }
        else {
            color = color * 1.0; //isnt dark bc takes care of weird right arm
        }
//
//    if (edge<0.01f){
//        //color black if normals are perpendicular to viewing direction
//        color = vec4(0.0f,0.0f,0.0f,1.0f);
//    }
//
    
    
}

