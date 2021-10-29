// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setShaders(std::string src,unsigned int index);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//MVP matrices
glm::mat4 Projection = glm::mat4(1.0);
glm::mat4 View = glm::mat4(1.0);
glm::mat4 Model = glm::mat4(1.0);

glm::vec3 position = glm::vec3(0,0,5);
float scale = 1.0f;
float rate = 0.3f;
float angle = 0.0f;
float angle2 = 0.0f;
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFoV = 45.0f;
float speed = 3.0f;
float mouseSpeed = 0.05f;
int shade = 0;
unsigned int shaders[4];
unsigned int shaderProgram;

glm::vec3 lightPos(1.0f,1.0f,-1.0f);
glm::vec3 lightColor(1.0f,1.0f,1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    
    //File io
    //Read shaders
    setShaders("source",0);
    setShaders("sourceZ",1);
    setShaders("phong",2);
    setShaders("gouraud",3);

    shaderProgram = shaders[shade];
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< float > temp_vertices;
    std::vector< float > temp_uvs;
    std::vector< float > temp_normals;
    
    FILE * file = fopen("cube.obj", "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }
    
    while(1){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if (strcmp(lineHeader,"v") == 0){
            float vertex[3];
            fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            temp_vertices.push_back(vertex[0]);
            temp_vertices.push_back(vertex[1]);
            temp_vertices.push_back(vertex[2]);
            float color[3];
            if(vertex[0] > 0) color[0] = vertex[0];
            else              color[0] = 0.0f;
            if(vertex[1] > 0) color[1] = vertex[1];
            else              color[1] = 0.0f;
            if(vertex[2] > 0) color[2] = vertex[2];
            else              color[2] = 0.0f;
            temp_vertices.push_back(color[0]);
            temp_vertices.push_back(color[1]);
            temp_vertices.push_back(color[2]);
        } else if(strcmp(lineHeader,"vt") == 0) {
            float uv[2];
            fscanf(file,"%f %f\n", &uv[0], &uv[1]);
            temp_uvs.push_back(uv[0]);
            temp_uvs.push_back(uv[1]);
        } else if(strcmp(lineHeader,"vn") == 0) {
            float normal[3];
            fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
            temp_normals.push_back(normal[0]);
            temp_normals.push_back(normal[1]);
            temp_normals.push_back(normal[2]);
        } else if(strcmp(lineHeader, "f") == 0){
            std::string vertex1, vertex2, vertex3;
            //unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            unsigned int vertexIndex[3], normalIndex[3];
            //int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
            if (matches != 6) {
                printf("File can't be read by my dumb parser\n");
                printf("%d",matches);
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]-1);
            vertexIndices.push_back(vertexIndex[1]-1);
            vertexIndices.push_back(vertexIndex[2]-1);
            //uvIndices    .push_back(uvIndex[0]-1);
            //uvIndices    .push_back(uvIndex[1]-1);
            //uvIndices    .push_back(uvIndex[2]-1);
            normalIndices.push_back(normalIndex[0]-1);
            normalIndices.push_back(normalIndex[1]-1);
            normalIndices.push_back(normalIndex[2]-1);
        }
    }
    
    //Handle vertices from vector to float
    float vertices[temp_vertices.size() + temp_normals.size()];
    int step = -1;
    for(int i = 0; i < temp_vertices.size() + temp_normals.size(); i++){
        if(i%9 == 0)
            step++;
        
        if((i%9) < 3)
            vertices[i] = temp_vertices[step*6 + (i%9)]/2;
        else if((i%9) < 6)
            vertices[i] = temp_vertices[step*6 + (i%9)];
        else
            vertices[i] = temp_normals[step*3 + (i%9-6)];
    }   
    unsigned int indices[vertexIndices.size()];
    for(int i = 0; i < vertexIndices.size(); i++){
        indices[i] = vertexIndices[i];
    }
    //Handle normals from vector to float
    //Note, normals have the same indices as vectors in cube.obj
    float normals[temp_normals.size()];
    for(int i = 0; i < temp_normals.size(); i++){
        normals[i] = temp_normals[i];
    }
    
    //Rotate the cube, outdated because we rotate by hand now
    //cos(pi/4) = 0.707
    //sin(pi/4) = 0.707
    //Rotate pi/4 in x and y
    // 0.707 0 0.707
    // 0.5 0.707 -0.5
    // -0.5 0.707 0.5
    //for(int i = 0; i < temp_vertices.size()/6; i++){
    //    vertices[i*6]   = 0.707 * temp_vertices[i*6]/2 + 0.707 * temp_vertices[i*6+2]/2;
    ///    vertices[i*6+1] = 0.5 * temp_vertices[i*6]/2 + 0.707 * temp_vertices[i*6+1]/2 - 0.5 * temp_vertices[i*6+2]/2;
    //    vertices[i*6+2] = -0.5 * temp_vertices[i*6]/2 + 0.707 * temp_vertices[i*6+1]/2 + 0.5 * temp_vertices[i*6+2]/2;
    //}
    
    unsigned int numVertices = sizeof(vertices)/6;

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //EBO stuff
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute?
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glm::mat4 MVP = Projection * View * Model;
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(shade < 2)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaders[shade], "MVP"), 1, GL_FALSE, &MVP[0][0]);
        }
        if(shade > 1)
        {
            glUniformMatrix4fv(glGetUniformLocation(shaders[shade], "Model"), 1, GL_FALSE, &Model[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaders[shade], "View"), 1, GL_FALSE, &View[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(shaders[shade], "Projection"), 1, GL_FALSE, &Projection[0][0]);
            
            glUniform3fv(glGetUniformLocation(shaders[shade],"lightPos"), 1, &lightPos[0]);
            glUniform3fv(glGetUniformLocation(shaders[shade],"viewPos"), 1, &position[0]);
            glUniform3fv(glGetUniformLocation(shaders[shade],"lightColor"), 1, &lightColor[0]);
        }
    
        // draw our first triangle
        glUseProgram(shaders[shade]);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    lastTime = currentTime;
    
    //Orientation
    double xpos, ypos;
    glfwGetCursorPos(window,&xpos,&ypos);
    
    glfwSetCursorPos(window,SCR_WIDTH/2, SCR_HEIGHT/2);
    
    //horizontalAngle += mouseSpeed * deltaTime * float(SCR_WIDTH/2 - xpos);
    //verticalAngle += mouseSpeed * deltaTime * float(SCR_HEIGHT/2 - ypos);
    angle += mouseSpeed * deltaTime * float(SCR_WIDTH/2 - xpos);
    angle2 -= mouseSpeed * deltaTime * float(SCR_HEIGHT/2 - ypos);
    
    
    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );
    
    glm::vec3 right = glm::vec3(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );
    
    glm::vec3 up = glm::cross(right, direction);
    
    //Scaling
    if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    {
        //position += direction * deltaTime * speed;
        scale += rate * deltaTime;
    }
    
    if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        //position -= direction * deltaTime * speed;
        scale -= rate * deltaTime;
    }
    
    if(glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS)
    {
        shade = 0;
    }
    if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS)
    {
        shade = 1;
    }
    if(glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS)
    {
        shade = 2;
    }
    if(glfwGetKey(window,GLFW_KEY_V) == GLFW_PRESS)
    {
        shade = 3;
    }
    
    //Incomprehensible rotation
    //if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
    //{
    //    angle += rate * deltaTime * speed;
    //}
    //
    //if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
    //{
    //    angle -= rate * deltaTime * speed;
    //}
    
    //FoV
    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(window); Function broken?
    
    //Calculate the matrices
    Model = glm::mat4(scale);
    Model[3][3] = 1.0f;
    
    Model = glm::rotate(Model,angle,glm::vec3(0.0f,1.0f,0.0f));
    Model = glm::rotate(Model,angle2,glm::vec3(1.0f,0.0f,0.0f));
    
    Projection = glm::perspective(glm::radians(FoV), 4.0f /3.0f, 0.1f, 100.0f);
    
    View = glm::lookAt(position,position+direction,up);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

float* readVertices(std::string s)
{
    float* p;
    return p;
}

unsigned int* readIndices(std::string s)
{
    unsigned int* p;
    return p;
}

void setShaders(std::string src, unsigned int index)
{
    unsigned int shaderProgram = glCreateProgram();
    std::ifstream in(src + ".vs");
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    const char* vertexShaderSource = contents.c_str();
    std::ifstream in2(src + ".fs");
    std::string contents2((std::istreambuf_iterator<char>(in2)),
                         std::istreambuf_iterator<char>());
    const char* fragmentShaderSource = contents2.c_str();
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    shaders[index] = shaderProgram;
}
