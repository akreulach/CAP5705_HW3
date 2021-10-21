#version 330 core
    out vec4 FragColor;
    in vec3 ourColor;
    in vec3 ourPos;
    void main(){
        float near = 0.1;
        float far = 5.0;
        //float z = ourPos.z * 2.0 - 1.0;
        //z = ((2.0 * near * far)/(far + near - z * (far - near))) / far;
        //FragColor = vec4(ourColor, 1.0f);
        float z = ourPos.z;
        z /= far;
        FragColor = vec4(vec3(1.0 - z), 1.0f);
    }
