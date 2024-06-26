#include "glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "glm/gtc/type_ptr.hpp"
#include <cstdlib>
#include <cstring>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
 



// Windows dimensions
const GLint width = 800, height = 800;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement =0.005f ;
bool rotate = true;
float triRotateOffset = 0.0f;
float triRotateMaxOffset = 3.14159265359f*2;
float triRotateIncrement =0.025f ;
GLuint VBO,VAO,EBO,VBO2,VAO2,shader_test,shader,uniformModel,uniformProjection,uniformModel_test;

// Vertex shader 
static const char* vertex_shader_text = 
"#version 330                                                      \n\
                                                                   \n\
layout ( location = 0 ) in vec3 pos;                               \n\
                                                                    \n\
 out vec4 vColor;                                                    \n\
 out vec4 vColor2;                                                    \n\
                                                                   \n\
uniform mat4 model;                                          \n\
uniform mat4 projection;                                          \n\
                                                                   \n\
void main()                                                        \n\
{                                                                  \n\
    gl_Position = projection * model * vec4(0.3*pos.x  ,0.3*pos.y ,0.3*pos.z, 1.0);        \n\
    vColor = vec4(clamp(pos,0.2f,1.0f), 1.0f);        \n\
       vColor2 = vec4(clamp(vec3(pos.y,pos.x,pos.z),0.2f,1.0f), 1.0f);        \n\
}";

// Fragment shader 
static const char* fragment_shader_text =
"#version 330                                                       \n\
                                                                   \n\
in vec4 vColor;                                                     \n\
out vec4 color;                                                    \n\
                                                                   \n\
void main()                                                        \n\
{                                                                  \n\
     color =vColor;                                                 \n\
}"
;
static const char* fragment_shader_text_test =
"#version 330                                                       \n\
                                                                   \n\
in vec4 vColor;                                                     \n\
out vec4 color;                                                    \n\
                                                                   \n\
void main()                                                        \n\
{                                                                  \n\
     color =vColor;                                                 \n\
}"
;
 
float toRadiant (float angle){
return angle*(3.14159265359/180.0);
}

void CreateTriangle( ){
 unsigned int elements[]={
0,3,4,
4,3,2,
 0,4,2,
0,3,1,
1,3,2,
// 2,3,0,
 0,1,2,

 };
    GLfloat vertices[]={
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
         0.0f, -1.0f, -1.0f,
    };
// for the previous rectangle along with vertices2
    // GLfloat vertices[]={
    //     -1.0f, -1.0f, 0.0f,
    //      1.0f, -1.0f, 0.0f,
    //      1.0f,  1.0f, 0.0f
    // };
       GLfloat vertices2[]={
        -1.0f,  -1.0f, 0.0f,
         -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    };
   glad_glGenVertexArrays(1,&VAO); 
   glad_glBindVertexArray(VAO);

   glad_glGenBuffers(1,&EBO);
   glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
   glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

   glad_glGenBuffers(1,&VBO);
   glad_glBindBuffer(GL_ARRAY_BUFFER,VBO);
   glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

   glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
   glad_glEnableVertexAttribArray(0);

   glad_glBindBuffer(GL_ARRAY_BUFFER,0);
   glad_glBindVertexArray(0);
   
   glad_glGenVertexArrays(1,&VAO2); 
   glad_glBindVertexArray(VAO2);

   glad_glGenBuffers(1,&VBO2);
   glad_glBindBuffer(GL_ARRAY_BUFFER,VBO2);
   glad_glBufferData(GL_ARRAY_BUFFER,sizeof(vertices2),vertices2,GL_STATIC_DRAW);

   glad_glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
   glad_glEnableVertexAttribArray(0);

   glad_glBindVertexArray(0);
   glad_glBindBuffer(GL_ARRAY_BUFFER,0);
   glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
   
   
    
}

void addShader(GLenum program,const char* shader_text, GLenum shader_type)
{
    GLuint the_shader = glad_glCreateShader(shader_type);
    const GLchar* shader_code[1] ;
    shader_code[0] = shader_text;

    GLint shader_code_length[1];
    shader_code_length[0] = strlen(shader_code[0]);

    glad_glShaderSource(the_shader,1,shader_code,shader_code_length);
    glad_glCompileShader(the_shader);

    GLint result = 0;
    GLchar eLog[1024]={0};

    glad_glGetShaderiv(the_shader,GL_COMPILE_STATUS,&result);

    if (!result) 
    {
        glad_glGetShaderInfoLog(the_shader,sizeof(eLog),NULL,eLog);
        printf("Error Compiling the %d program !! :\n '%s' \n",shader_type ,eLog);
    return;
    }

     glad_glAttachShader(program,the_shader);
}

void compileShaders()
   {
    
    shader = glad_glCreateProgram();
    shader_test = glad_glCreateProgram();

    if(!shader)
    {
        printf("Error creating shader program!\n");    
        return;
    }

    addShader(shader, vertex_shader_text, GL_VERTEX_SHADER);
    addShader(shader, fragment_shader_text, GL_FRAGMENT_SHADER);
    addShader(shader_test, vertex_shader_text, GL_VERTEX_SHADER);
    addShader(shader_test, fragment_shader_text_test, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024]={0};

    glad_glLinkProgram(shader);
    glad_glLinkProgram(shader_test);
    glad_glGetProgramiv(shader,GL_LINK_STATUS,&result);

    if (!result) 
    {
        glad_glGetProgramInfoLog(shader,sizeof(eLog),NULL,eLog);
        printf("Error linking program !! : '%s' \n",eLog);
        return;
    }

    glad_glValidateProgram(shader);
    glad_glValidateProgram(shader_test);
    glad_glGetShaderiv(shader,GL_VALIDATE_STATUS,&result);

    if (!result) 
    {
        glad_glGetProgramInfoLog(shader,sizeof(eLog),NULL,eLog);
        printf("Error Validating program !! : '%s' \n",eLog);
    return;
    }

    uniformModel = glad_glGetUniformLocation(shader,"model");
    uniformModel_test = glad_glGetUniformLocation(shader_test,"uniformModel");
    uniformProjection = glad_glGetUniformLocation(shader,"projection");
   }
  

int main(void)
{

    if (!glfwInit()) {
        printf("Error Initialize GLFW !\n");    
        glfwTerminate();
        return 1;
    }

    
// glad_glDepthFunc(GL_LEQUAL);
// glad_glDepthMask();
    // Setup GLFW Windows Properties
    // openGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create the window
    GLFWwindow *main_window = glfwCreateWindow( width,  height, "Main window", NULL,NULL);

    if (!main_window) {
        printf("Error Creating GLFW Window !\n");    
        glfwTerminate();
        return 1;
    }

    // Get buffer size informations
    int buffer_width, buffer_height = 500;
    glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

    // Set current context
    glfwMakeContextCurrent(main_window);
     gladLoadGL(); 
   

    // Create viewport 
    glad_glViewport(0,0,buffer_width,buffer_height);

    CreateTriangle();
     
    compileShaders();
  
 glEnable(GL_DEPTH_TEST);
   glm::mat4 projection ;
    projection = glm::perspective(45.0f, (GLfloat)buffer_width/(GLfloat)buffer_height,0.1f,100.0f);
    // Loop until window close 
    while (!glfwWindowShouldClose(main_window)) {
    // Get + handle user input
    glfwPollEvents();

    // Moving the triangle 
     if (direction) {
     triOffset+=triIncrement;
     }else {
     triOffset-=triIncrement;
     }

     if (std::abs(triOffset)>=triMaxOffset) {
     direction =!direction;
     }
     if (rotate) {
     triRotateOffset+=triRotateIncrement;
     }else {
     triRotateOffset-=triRotateIncrement;
     }

     if (triRotateOffset>=triMaxOffset||triRotateOffset==0) {
     rotate =!rotate;
     }

    // Clear the window
    glad_glClearColor(0.0f,0.0f,0.0f,1.0f);
    
     glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glad_glUseProgram(shader);
   
    glm::mat4 model(1) ;
     model = glm::translate(model, glm::vec3(0.0f,0.0f,-2.5f));
    model = glm::rotate(model, triRotateOffset,glm::vec3(1.0f,1.0f,1.0f));
    
    
    // model = glm::scale(model, glm::vec3(triOffset,triOffset,0.0f));
    

  
    glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection,1,GL_FALSE,glm::value_ptr(projection));
    
    glad_glBindVertexArray(VAO);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    glad_glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_INT,0);

    // glad_glBindVertexArray(0);
    
 
  
    // glad_glUseProgram(shader_test);
    // glad_glBindVertexArray(VAO2);
   
  
    // glUniformMatrix4fv(uniformModel_test,1,GL_FALSE,glm::value_ptr(model));
    // glad_glDrawArrays(GL_TRIANGLES,0,3);

    glad_glBindVertexArray(0);
    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glad_glUseProgram(0);

    

    glfwSwapBuffers(main_window);



    } 
 
 
}






























 
// static const struct
// {
//     float x, y;
//     float r, g, b;
// } vertices[3] =
// {
//     { -1.f, -1.f, 1.f, 0.f, 0.f },
//     {  1.f, -1.f, 0.f, 1.f, 0.f },
//     {   0.f,  1.f, 0.f, 0.f, 1.f }
// };
 
// static const char* vertex_shader_text =
// "#version 110\n"
// "uniform mat4 MVP;\n"
// "attribute vec3 vCol;\n"
// "attribute vec2 vPos;\n"
// "varying vec3 color;\n"
// "void main()\n"
// "{\n"
// "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
// "    color = vCol;\n"
// "}\n";
 
// static const char* fragment_shader_text =
// "#version 110\n"
// "varying vec3 color;\n"
// "void main()\n"
// "{\n"
// "    gl_FragColor = vec4(color, 1.0);\n"
// "}\n";
 
// static void error_callback(int error, const char* description)
// {
//     fprintf(stderr, "Error: %s\n", description);
// }
 
// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
// }
 
// int main(void)
// {
//     GLFWwindow* window;
//     GLuint vertex_buffer, vertex_shader, fragment_shader, program;
//     GLint mvp_location, vpos_location, vcol_location;
 
//     glfwSetErrorCallback(error_callback);
 
//     if (!glfwInit())
//         exit(EXIT_FAILURE);
 
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
//     window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
//     if (!window)
//     {
//         glfwTerminate();
//         exit(EXIT_FAILURE);
//     }
 
//     glfwSetKeyCallback(window, key_callback);
 
//     glfwMakeContextCurrent(window);
//     gladLoadGL();
//     glfwSwapInterval(1);
 
//     // NOTE: OpenGL error checks have been omitted for brevity
 
//     glGenBuffers(1, &vertex_buffer);
//     glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
//     vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
//     glCompileShader(vertex_shader);
 
//     fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
//     glCompileShader(fragment_shader);
 
//     program = glCreateProgram();
//     glAttachShader(program, vertex_shader);
//     glAttachShader(program, fragment_shader);
//     glLinkProgram(program);
 
//     mvp_location = glGetUniformLocation(program, "MVP");
//     vpos_location = glGetAttribLocation(program, "vPos");
//     vcol_location = glGetAttribLocation(program, "vCol");
 
//     glEnableVertexAttribArray(vpos_location);
//     glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
//                           sizeof(vertices[1]), (void*) 0);
//     glEnableVertexAttribArray(vcol_location);
//     glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
//                           sizeof(vertices[1]), (void*) (sizeof(float) * 2));
 
//     while (!glfwWindowShouldClose(window))
//     {
//         float ratio;
//         int width, height;
//         mat4x4 m, p, mvp;
 
//         glfwGetFramebufferSize(window, &width, &height);
//         ratio = width / (float) height;
 
//         glViewport(0, 0, width, height);
//         glClear(GL_COLOR_BUFFER_BIT);
 
//         mat4x4_identity(m);
//         mat4x4_rotate_Z(m, m, (float) glfwGetTime());
//         mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//         mat4x4_mul(mvp, p, m);
 
//         glUseProgram(program);
//         glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
//         glDrawArrays(GL_TRIANGLES, 0, 3);
 
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }
 
//     glfwDestroyWindow(window);
 
//     glfwTerminate();
//     exit(EXIT_SUCCESS);
// }