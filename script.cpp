#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "script.h"

void Start() {
    holamundo.CreateQuad(30,30);
    holamundo2.CreateQuad(30,10);
    holamundo2.Position.x = -90;
    holamundo2.Position.y = 0;
    holamundo2.Rotation.z = 90;
    Window = glfwGetCurrentContext();
}

void Update() {
    float moveSpeed = 200.0f * deltaTime();
    
    if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
        holamundo.Position.y += moveSpeed;
    }
    if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
        holamundo.Position.y -= moveSpeed;
    }
    if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
        holamundo.Position.x -= moveSpeed;
    }
    if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
        holamundo.Position.x += moveSpeed;
    }
    
    if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        holamundo2.Position += Vector2(1,0);
    }
    
    colision = holamundo2.CheckCollision();
    if(colision.Bool)
    {
        holamundo2.Position.x -= 2;
    }
    
    std::cout << "Posición holamundo: X=" << holamundo.Position.x << ", Y=" << holamundo.Position.y << std::endl;
}