#version 330 core
    out vec4 FragColor;
    
    in vec3 normal;
    in vec3 fragPos;
    in vec3 ourColor;
    
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    
    void main(){
        float ambientIntensity = 0.2;
        vec3 ambient = ambientIntensity * lightColor;
        
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm,lightDir),0.0);
        vec3 diffuse = diff * lightColor;
    
        float specularIntensity = 1.0;
        vec3 viewDir = normalize(-viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir,norm);
        float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
        vec3 specular = specularIntensity * spec * lightColor;
    
        FragColor = vec4((ambient+diffuse+specular) * ourColor, 1.0f);
    }
