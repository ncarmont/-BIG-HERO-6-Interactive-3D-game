#version 330 core

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 model;
uniform mat4 view;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D tex;

in vec2 fragTexCoord; //texture coord
in vec3 fragNormal; //normal
in vec3 fragVert; //position
out vec4 color;


void main()
{

    //normal in world coordinates
    vec3 normal = normalize(transpose(inverse(mat3(modelview))) * fragNormal);
    
    vec3 lighting = vec3(modelview * vec4(lightPos, 1.0f));
    vec3 cam = vec3(modelview * vec4(cameraPos, 1.0f));
    vec3 vert = vec3(modelview * vec4(fragVert, 1.0f));
    
//interesting colors to look at, glossy tinny feel
//    float towardsLight = dot(normal, lightPos);
//    float towardsLightChange = fwidth(towardsLight);
//    float intensity = smoothstep(0,towardsLightChange, towardsLight);
    
    
   // diffuse lighting?
    vec3 surfaceToLight = normalize(lighting - vert);
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    vec4 surfaceColor = texture(tex, fragTexCoord);
    color = vec4(brightness * lightColor * surfaceColor.rgb, surfaceColor.a);
    
//    color = texture(tex, fragTexCoord);
    
    
    
    //silhouette edges and discretized shading
    vec3 incident_eye = normalize(vert);//do i need to -
    vec3 n = normalize(normal);         //normal
    vec3 v = normalize(cam - vert);     //viewing direction
    vec3 l = normalize(lighting - vert);//light direction

    float edge = max(0.0f,dot(n,v));
    float diffuse = max(0.0f,dot(n,l));

    //shading
    if (diffuse > 0.75) {
        color = color * 1.0;
    }
    else if (diffuse > 0.5){
        color = color * 0.8;
    }
    else if (diffuse > 0.3) {
        color = color * 0.3;
    }
    else {
        color = color * 0.2;
    }

    //edges
    if (edge<0.01f){
        //color black if normals are perpendicular to viewing direction
        color = vec4(0.0f,0.0f,0.0f,1.0f);
    }
    
//    color = texture(tex, fragTexCoord);
    
    
}


