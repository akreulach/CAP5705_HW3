#version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;
    layout (location = 2) in vec3 aNormal;
    out vec3 ourColor;
    out vec3 ourPos;
    uniform mat4 MVP;
    void main(){
       gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
       ourColor = aCol;
       ourPos = gl_Position.xyz;
    }
