#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Anvil.h"

//====================IMPORTANTE=====================   
//Si vas a usar este script renombrar a "script.h" y agregarlo al proyecto
//El motor lo va a compilar y ejecutar automaticamente


//===================================================
//=====================**PONG**======================
//==================EN FORGE ENGINE==================
//===================================================


//=======================Controles========================
/**
 * D & RIGHT -> Movimiento a la derecha
 * A & LEFT -> Movimiento a la izquierda
 * SPACE -> Salto
 * Si se mantiene C -> Ir mas rapido
 */

//=======================Definicion de objetos globales========================
//Inicializamos los objetos y las variables globales que vamos a usar en el script
//No se preocupen por esto, es solo para que el motor pueda acceder a estas variables y objetos desde el script


AnvilObject Suelo;
AnvilObject Personaje;
GLFWwindow* Window;
Collision2D colision;

float velocidadY = 0.0f;
float speedplayer = 80.0f;
float gravedad = -9.81f; //Gravedad de la tierra

/**
 * Funcion donde se encuentra la lógica de las Fisicas
 */
void PhysicsUpdate() {
    float dt = deltaTime();

    velocidadY += gravedad * dt;                    // Aplicar gravedad a la velocidad vertical

    float predictedY = Personaje.Position.y + velocidadY * dt;  // Predecir la nueva posicion
    float personajeBottom = predictedY - (Personaje.Height / 2.0f);  // Calcular el borde inferior predicho
    float sueloTop = Suelo.Position.y + (Suelo.Height / 2.0f);  // Calcular el borde superior del suelo

    // Si el personaje va a tocar o pasar el suelo
    if (personajeBottom <= sueloTop) {
        Personaje.Position.y = sueloTop + (Personaje.Height / 2.0f); // Posicionar el centro del personaje justo encima del suelo
        velocidadY = 0.0f; // Aterrizaje
    } else {
        Personaje.Position.y = predictedY;  
    }
}

void Start() {
    Suelo.CreateQuad(1080, 3);
    Personaje.CreateQuad(10, 10);

    Suelo.Position = Vector2(0.0f, -100.0f);
    Personaje.Position = Vector2(0.0f, 0.0f);

    Window = glfwGetCurrentContext();

    //COLORS
    Personaje.color = Color(1.0f, 100.0f, 0.0f);
    Suelo.color = Color(5.0f, 0.0f, 0.0f);
    
}

void Update() {
    std::cout << "Posicion del jugador: (" << Personaje.Position.x << ", " << Personaje.Position.y << ")" << std::endl;
    float moveSpeed = (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS) ? 150.0f : 80.0f;
    
    if ((glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)) {
        Personaje.Position.x += moveSpeed * deltaTime();
    }
    if ((glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)) {
        Personaje.Position.x -= moveSpeed * deltaTime();
    }
    if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS && velocidadY == 0.0f) {
        velocidadY = 20.0f;
    }

    //Hay que declararlo para que funcione
    PhysicsUpdate();
}

