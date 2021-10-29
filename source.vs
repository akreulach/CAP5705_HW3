#version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;
    layout (location = 2) in vec3 aNormal;
    out vec3 ourColor;
    uniform mat4 MVP;
    void main(){
       gl_Position = MVP * vec4(aPos, 1.0);
       ourColor = aCol;
    }
