#version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;
    layout (location = 2) in vec3 aNormal;
    
    out vec3 fragPos;
    out vec3 ourColor;
    out vec3 normal;
    
    uniform mat4 Model;
    uniform mat4 View;
    uniform mat4 Projection;
    
    void main(){
        fragPos = vec3(Model * vec4(aPos, 1.0));
        normal = mat3(transpose(inverse(Model))) * aNormal;
    
        gl_Position = Projection * View * vec4(fragPos, 1.0);
        ourColor = aCol;
    }
