#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <algorithm>
#include <list>

const GLchar *vertexShaderSource = R"glsl(
#version 150 core
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
in vec3 position;
in vec3 color;
out vec3 Color;
void main(){
Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
//gl_Position = vec4(position, 1.0);
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
#define W_HEIGHT 640

#define VERTEX_DATA_COUNT 6
static float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};
static size_t triangles = sizeof(vertices) / (sizeof(float) * VERTEX_DATA_COUNT * 3);

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view;

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

float cam_angle = 0.0f;
float cam_angle2 = 0.0f;

std::list<int> keys_pressed;

GLint uniView;

void move_camera(GLFWwindow *window) {
    constexpr float cam_speed = 0.0001f;
    std::for_each(keys_pressed.begin(), keys_pressed.end(), [window](int key) {
        switch (key) {
            case (GLFW_KEY_ESCAPE):
                glfwSetWindowShouldClose(window, true);
                break;
            case (GLFW_KEY_KP_ADD):
                camera_pos += cam_speed * camera_front;
                break;
            case (GLFW_KEY_KP_SUBTRACT):
                camera_pos -= cam_speed * camera_front;
                break;
            case (GLFW_KEY_W):
                camera_pos.y += cam_speed;
                break;
            case (GLFW_KEY_S):
                camera_pos.y -= cam_speed;
                break;
            case (GLFW_KEY_A):
                camera_pos.x -= cam_speed;
                break;
            case (GLFW_KEY_D):
                camera_pos.x += cam_speed;
                break;
            case (GLFW_KEY_LEFT):
                cam_angle -= cam_speed;
                camera_front.x = sinf(cam_angle);
                camera_front.z = -cosf(cam_angle);
                break;
            case (GLFW_KEY_RIGHT):
                cam_angle += cam_speed;
                camera_front.x = sinf(cam_angle);
                camera_front.z = -cosf(cam_angle);
                break;
            case (GLFW_KEY_Q):
                cam_angle2 += cam_speed;
                camera_up.x = sinf(cam_angle2);
                camera_up.z = -cosf(cam_angle2);
                break;
            case (GLFW_KEY_E):
                cam_angle2 -= cam_speed;
                camera_up.x = sinf(cam_angle2);
                camera_up.z = -cosf(cam_angle2);
                break;
            default:
                break;
        }
    });
    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto result = std::find(keys_pressed.begin(), keys_pressed.end(), key);
    if (action == GLFW_PRESS && result == keys_pressed.end()) {
        keys_pressed.push_back(key);
    }
    if (action == GLFW_RELEASE) {
        keys_pressed.remove(key);
    }
}

int main() {
    std::cout << triangles << std::endl;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangles * VERTEX_DATA_COUNT * 3, vertices, GL_STATIC_DRAW);

    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

    GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

    uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_COUNT * sizeof(GLfloat), nullptr);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, VERTEX_DATA_COUNT * sizeof(GLfloat),
                          (void *) (3 * sizeof(GLfloat)));

    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    glfwSetKeyCallback(window, key_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, triangles * 3);
        move_camera(window);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}