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

//=======================Definicion de objetos globales========================
//Inicializamos los objetos y las variables globales que vamos a usar en el script
//No se preocupen por esto, es solo para que el motor pueda acceder a estas variables y objetos desde el script
AnvilObject Pala;
AnvilObject Pala2;
AnvilObject Pelota;
GLFWwindow* Window;
Collision2D colision1;
Collision2D colision2;

//=======================Funciones del script========================
float speedPelota = 100.0f;
float mulXPelota = 1.0f;
float mulYPelota = 1.0f;

// Variables para la IA de la paleta 2
float iaSpeed = 120.0f;
float iaErrorChance = 0.15f;
float iaErrorTimer = 0.0f;
bool iaMakingError = false;
float iaErrorDirection = 1.0f;

void Start() {
    Pala.CreateQuad(2, 20);
    Pala2.CreateQuad(2, 20);
    Pelota.CreateQuad(1,1);
    Pala.Position = Vector2(-100.0f, 0.0f);
    Pala2.Position = Vector2(100.0f, 0.0f);
    Window = glfwGetCurrentContext();
    SetWindowTitle("PONG 1.0");
}
//h

void Update() {
    if (iaMakingError) {
        iaErrorTimer -= deltaTime();
        if (iaErrorTimer <= 0.0f) {
            iaMakingError = false;
        }
    }
    
    if (!iaMakingError && (rand() % 100) < (iaErrorChance * 100)) {
        iaMakingError = true;
        iaErrorTimer = 0.3f + (rand() % 100) / 200.0f;
        iaErrorDirection = (rand() % 2) == 0 ? 1.0f : -1.0f;
    }
    
    float targetY = Pelota.Position.y;
    float currentY = Pala2.Position.y;
    float diff = targetY - currentY;
    
    if (iaMakingError) {
        diff *= iaErrorDirection * 0.5f;
    }
    
    if (abs(diff) > 5.0f) {
        if (diff > 0 && Pala2.Position.y < 80.0f) {
            Pala2.Position.y += iaSpeed * deltaTime();
        } else if (diff < 0 && Pala2.Position.y > -80.0f) {
            Pala2.Position.y -= iaSpeed * deltaTime();
        }
    }
    std::cout << "Posicion de la pelota: (" << Pelota.Position.x << ", " << Pelota.Position.y << ")" << std::endl;
    if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS && Pala.Position.y < 80.0f) {
        Pala.Position.y += 150.0f * deltaTime();
    }
    if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS && Pala.Position.y > -80.0f) {
        Pala.Position.y -= 150.0f * deltaTime();
    }
    Pelota.Position += Vector2((speedPelota * mulXPelota) * deltaTime(), (speedPelota * mulYPelota) * deltaTime());
    if (Pelota.Position.y > 100.0f) {
        mulYPelota *= -1;
        Pelota.Position.y = 100.0f; // Evita que la pelota se quede fuera de los límites
    }
    else if (Pelota.Position.y < -100.0f) {
        mulYPelota *= -1;
        Pelota.Position.y = -100.0f; // Evita que la pelota se quede fuera de los límites
    }
    if (Pelota.Position.x > 150.0f)
    {
        mulXPelota *= -1;
    }
    colision1 = Pelota.CheckCollision();
    if (colision1.Bool)
    {
        mulXPelota *= -1;

        
    }


    if (Pelota.Position.x < -150.0f)
    {
        mulXPelota = 1.0f;
        mulYPelota = 1.0f;
        Pelota.Position = Vector2 (0.0f, 0.0f);
    }


}