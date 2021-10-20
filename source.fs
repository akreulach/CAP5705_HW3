#version 330 core
    out vec4 FragColor;
    in vec3 ourColor;
    void main(){
        float near = 0.1;
        float far = 20.0;
        float z = gl_FragCoord.z * 2.0 - 1.0;
        z = ((2.0 * near * far)/(far + near - z * (far - near))) / far;
        //FragColor = vec4(ourColor, 1.0f);
        FragColor = vec4(vec3(z), 1.0f);
    }
