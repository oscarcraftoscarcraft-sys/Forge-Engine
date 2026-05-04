#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "../Anvil.h"

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

void Start() {
    Pala.CreateQuad(2, 20);
    Pala2.CreateQuad(2, 20);
    Pelota.CreateQuad(1,1);
    Pala.Position = Vector2(-100.0f, 0.0f);
    Pala2.Position = Vector2(100.0f, 0.0f);
    
    
    Window = glfwGetCurrentContext();

    //COLORS
    Pala.color = Color(1.0f, 1.0f, 0.0f);
    Pala2.color = Color(1.0f, 0.0f, 0.0f);
    Pelota.color = Color(1.0f, 1.0f, 0.0f);

    //Añadir una textura al objeto
}

void Update() {
    
    std::cout << "Posicion de la pelota: (" << Pelota.Position.x << ", " << Pelota.Position.y << ")" << std::endl;

    if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) {
        Pala.Position.y += 150.0f * deltaTime();
    }
    if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) {
        Pala.Position.y -= 150.0f * deltaTime();
    }
    //Paleta 2 (derecha) - controles I/K
    if (glfwGetKey(Window, GLFW_KEY_I) == GLFW_PRESS) {
        Pala2.Position.y += 150.0f * deltaTime();
    }
    if (glfwGetKey(Window, GLFW_KEY_K) == GLFW_PRESS) {
        Pala2.Position.y -= 150.0f * deltaTime();
    }
    colision1 = Pelota.CheckCollision();
    if (colision1.Bool)
    {
        colision1.Object->color = Color(0.0f, 1.0f, 0.0f);
        mulXPelota *= -1;
    }
    Pelota.Position += Vector2((speedPelota * mulXPelota) * deltaTime(), (speedPelota * mulYPelota) * deltaTime());
    if (Pelota.Position.y > 100.0f) {
        mulYPelota *= -1;
    }
    else if (Pelota.Position.y < -100.0f) {
        mulYPelota *= -1;
    }
    if (Pelota.Position.x > 150.0f)
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