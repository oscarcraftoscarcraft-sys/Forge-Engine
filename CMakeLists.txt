cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(ForgeEngine)

# Forzamos el estándar C++23 como tienes en VS Code
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Evitar el aviso de OpenGL (preferir la versión moderna GLVND)
set(OpenGL_GL_PREFERENCE GLVND)

# 1. Buscar las librerías necesarias en el sistema
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# 2. Definir los archivos fuentes
# Solo incluimos el Main porque tú usas #include "script.cpp" dentro de él.
# Esto emula el comportamiento de "archivo único" de VS Code.
set(SOURCES
    ForgeEngineMain.cpp
)

# 3. Crear el ejecutable
add_executable(Forge2Engine ${SOURCES})

# 4. Indicar dónde están los archivos de cabecera (.h)
target_include_directories(Forge2Engine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLEW_INCLUDE_DIRS}
)

# 5. Enlazar las librerías al ejecutable (el -lGLEW -lglfw etc. de VS Code)
target_link_libraries(Forge2Engine PRIVATE 
    OpenGL::GL
    GLEW::GLEW
    glfw

)