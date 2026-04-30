#include <iostream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "Anvil.h"


AnvilObject Template;

void Start() {
    Template.CreateQuad(20, 20);
}

void Update() {}