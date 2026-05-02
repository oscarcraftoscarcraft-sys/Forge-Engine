#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 proyeccion;
uniform mat4 rotacion;
uniform mat4 translacion;
uniform mat4 escala;

void main()
{
    gl_Position = proyeccion * translacion * rotacion * escala * vec4(aPos, 1.0);
}