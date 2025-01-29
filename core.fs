#version 330 core
out vec4 FragColor;
in vec3 VertexColor;
in vec3 Normal ;
in vec3 FragPos;

uniform vec3 LightPos;
uniform vec3 viewPos;
void main()
{
    //ambient
    vec3 ambient = 0.4f * VertexColor;

    //diffuse
    vec3 norm = normalize(Normal);
 
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = 0.7f * max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * VertexColor;

    //specular
     

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfAngle = normalize(viewDir + lightDir);
    float spec = 0.9f * pow(max(dot(norm, halfAngle), 0.0f), 64);
    vec3 specular =  spec * VertexColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}