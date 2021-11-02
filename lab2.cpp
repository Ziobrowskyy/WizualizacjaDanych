#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLchar *vertexShaderSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
out vec3 Color;
void main(){
Color = color;
gl_Position = vec4(position, 1.0);
}
)glsl";

const GLchar *fragmentShaderSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;
void main()
{
outColor = vec4(Color, 1.0);
}
)glsl";

#define W_WIDTH 640
#define W_HEIGHT 480

#define TRIANGLES 2

GLfloat vertices[] = {
        0.5f, 0.5f, -1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.1f,0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.1f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.1f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.1f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.1f,0.0f, 1.0f, 0.0f,
};

int main() {

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /* Create a windowed mode window and its OpenGL context */
    glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", nullptr, nullptr);

    if (!window) {
        std::cout << "Could not create instance of GLFWwindow. Terminating...";
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLuint VAO{0}, VBO{0};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.21f, 0.37f, 0.69f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, TRIANGLES * 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}