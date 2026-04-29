#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Anvil.h"
#include "script.h"
#include <vector>

AnvilObject holamundo;
AnvilObject holamundo2;
GLFWwindow* Window;
Collision2D colision;

void error_callback(int error, const char* description) {
    std::cerr << "Error de GLFW (" << error << "): " << description << std::endl;
}



int main() {
    

    glfwSetErrorCallback(error_callback);
    // ==================== Inicializar GLFW ====================
    if (!glfwInit()) {
        std::cout << "Error: No se pudo inicializar GLFW" << std::endl;
        return -1;
    }


    // Crear la ventana
    
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Mi Primera Ventana OpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Error: No se pudo crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Hacer el contexto de OpenGL actual
    glfwMakeContextCurrent(window);

    // ==================== Inicializar GLEW ====================
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error: No se pudo inicializar GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    //shader, recibe las matrices y las aplica a la posicion final
    //un poquito mejor explicado mas adelante
    const char *vertexshaderorigen = "#version 330\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location= 1) in vec3 color;\n"
        "uniform mat4 proyeccion;\n"
        "uniform mat4 rotacion;\n"
        "uniform mat4 translacion;\n"
        "uniform mat4 escala;\n"
        "out vec3 color2;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = proyeccion * translacion * rotacion * escala* vec4(aPos, 1.0);\n"
        "   color2 = color;\n"
        "}\0";
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexshaderorigen, NULL);
    glCompileShader(vertexShader);
    //recibe el color del vertex y lo aplica
    const char *fragmentShaderOrigen = "#version 330\n"
        "out vec4 fragColor;\n"
        "in vec3 color2;\n"
        "void main()\n"
        "{\n"
        "   fragColor = vec4(color2, 1.0f);\n"
        "}\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderOrigen, NULL);
    glCompileShader(fragmentShader);
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 1000, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    float aspect = 1200.0f / 800.0f;
    float matrizProye[] = {
        1.0f / aspect, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float matrizRotacion[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float matrizTraslacion[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float matrizEscala[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 1.0f
    };
    auto& lista = InternalPassVerticesList();
    auto& objetosLista = InternalPassReferenceObjects();
    int idUniform = glGetUniformLocation(shaderProgram, "proyeccion");
    int rotacionID = glGetUniformLocation(shaderProgram, "rotacion");
    int translacionID = glGetUniformLocation(shaderProgram, "translacion");
    int escalaID = glGetUniformLocation(shaderProgram, "escala");
    float ultimoframe;
    glUniformMatrix4fv(idUniform, 1, GL_FALSE, matrizProye);
    Start();
    // ==================== Bucle principal ====================
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        Update();
        //bucle principal, las listas que definimos en Anvil.h las recorre agarra los vertices y los renderiza
        //ademas (pendiente por corregir) si los objetos y los vertices estan alineados (en esta version suponemos que si)
        //aplicamos estas propiedades al shader de los objetos en la lista y dibujamos
        //y esto se vuelve a repetir con todos los objetos en las listas creados
        for(size_t i = 0; i < lista.size(); i++) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, lista[i].size() * sizeof(float), lista[i].data());
            matrizTraslacion[12] = objetosLista[i]->Position.x / 100.0f;
            matrizTraslacion[13] = objetosLista[i]->Position.y / 100.0f;
            if (objetosLista[i]->Rotation.z >= 360) {
                objetosLista[i]->Rotation.z = 0;
            }
            matrizRotacion[0] = cos((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[1] = -sin((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[4] = sin((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizRotacion[5] = cos((objetosLista[i]->Rotation.z) * (M_PI / 180));
            matrizEscala[0] = objetosLista[i]->Scale.x / 100;
            matrizEscala[5] = objetosLista[i]->Scale.y / 100;
            glUniformMatrix4fv(rotacionID, 1, GL_FALSE, matrizRotacion);
            glUniformMatrix4fv(translacionID, 1, GL_FALSE, matrizTraslacion);
            glUniformMatrix4fv(escalaID, 1, GL_FALSE, matrizEscala);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, lista[i].size()/6);
        }   

        float primerFrame = glfwGetTime();
        float deltatime = primerFrame - ultimoframe;
        ultimoframe = primerFrame;
        InternalPassDontAsk(deltatime);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ==================== Limpiar ====================
    glfwTerminate();
    return 0;
}