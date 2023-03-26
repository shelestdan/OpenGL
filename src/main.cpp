#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 in_color;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "   color = in_color;"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 color;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(color, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit(); //Инифиализация библиотеки GLFW, перед использованием других функций. Эта функция обязательно должна быть завершена по окончанию работы
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Это функция подсказки, для вызова функции glfwCreateWindow.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //Функция создания окна
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //Эта функция делает контекст указанного окна. Иначе говоря - всё, что мы хотим отрисовать будет происходить на этом окне
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Функция для изменения окна.

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //Подключение библиотеки GLAD
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //Создаёт объект шейдера, параметром является тип шейдера - VERTER, GEOMETRY, FRAGMENT.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Устанавливает исходный код в Shaders на исходный код в массиве строк. 
    glCompileShader(vertexShader); //Компилирует объект шейдера. 

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //Возвращает параментр из объета шейдера. 
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); //Возвращает информационный журнал, в запрашиваемом шейдере.
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); // Прикрепляет указанный шейдер к программе.
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // Соединяет указанные шейдеры.

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // Генерирует имена объектов массива вершин.
    glGenBuffers(1, &VBO); // Генерирует имена буфера вершин. 

    glBindVertexArray(VAO); // Привязка объекта массива вершин.
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязка объекта буфера.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Создаёт и инициалирует хранилище данных буферного объекта. 

    glEnableVertexAttribArray(0); // Включает или отключает общий массив атрибутов вершин. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Определяет массив общих данных атрибута вершин.

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}