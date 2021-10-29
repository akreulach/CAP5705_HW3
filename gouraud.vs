#version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;
    layout (location = 2) in vec3 aNormal;
    
    out vec3 ourColor;
    
    uniform mat4 Model;
    uniform mat4 View;
    uniform mat4 Projection;
    
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    
    void main(){
        vec3 fragPos = vec3(Model * vec4(aPos, 1.0));
        vec3 normal = mat3(transpose(inverse(Model))) * aNormal;
    
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
    
        ourColor = (ambient+diffuse+specular) * aCol;
    
        gl_Position = Projection * View * vec4(fragPos, 1.0);
    }
