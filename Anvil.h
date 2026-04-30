#pragma once
#include <GL/glew.h>    // ¡Siempre antes de GLFW!
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <math.h>

class AnvilObject;


//listas para que el motor las pueda recorrer y renderizar y aplicar sus propiedades como posicion, rotacion etc....
extern std::vector<std::vector<float>> cosasARenderizar;
extern std::vector<AnvilObject*> objetosReferenciados;
//delta time oculto para pasar desde el motor
extern float dontAskWhy;

struct Vector2 {
    float x = 0.0f;
    float y = 0.0f;
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2 operator+(const Vector2& other) {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2 operator-(const Vector2& other) {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2 operator*(const Vector2& other) {
        return Vector2(x * other.x, y * other.y);
    }
    Vector2 operator*=(const Vector2& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }
    Vector2 operator/(const Vector2& other) {
        return Vector2(x / other.x, y / other.y);
    }
    Vector2 operator/=(const Vector2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
};


struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3 operator+(const Vector3& other) {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
};

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}
    Color operator+(const Color& other) {
        return Color(r + other.r, g + other.g, b + other.b);
    }
};

class AnvilObject;

class Collision2D {
    public:
    bool Bool;
    AnvilObject* Object;
};


class AnvilObject {
    //tiene sus propiedades que luego el motor las procesa y las manda a la matriz del shader
    private: 
    int _Width, _Height;
    public:
        int Width, Height;
        Vector2 Position;
        Vector3 Rotation;
        Color color;
        Vector2 Scale = Vector2(100.0f, 100.0f);
    //agrega el objeto creado a una lista para luego ser recorrido y aplicar sus propiedad al shader
    AnvilObject() {
        objetosReferenciados.push_back(this);
    }
    //crea un cuadrado con los parametros ancho y alto
    //hace una media entre el alto y el ancho para que la forma este centrada
    //luego esto se envia a una lista donde el motor lo podra meter en el buffer central, recorre toda la lista y lo renderiza
    // (solo hay un buffer, no pregunten)
    void CreateQuad(int width, int height)
    {
        float medidaAncho = width / 50.0f; 
        float medidaAlto  = height / 50.0f;

        float mitadAncho = medidaAncho / 2.0f;
        float mitadAlto  = medidaAlto / 2.0f;
        Width=width; 
        Height=height;
        std::vector<float> ListaVertices = {
            -mitadAncho,  mitadAlto, 0.0f, 1.0f, 1.0f, 1.0f, // Arriba-Izquierda
            -mitadAncho, -mitadAlto, 0.0f, 1.0f, 1.0f, 1.0f, // Abajo-Izquierda
            mitadAncho,  mitadAlto, 0.0f, 1.0f, 1.0f, 1.0f, // Arriba-Derecha
            mitadAncho, -mitadAlto, 0.0f, 1.0f, 1.0f, 1.0f  // Abajo-Derecha
        };
        cosasARenderizar.push_back(ListaVertices);
    };
    // Falta por arreglar
    // Sistema abba con adaptacion a la rotacion (no funcionaba)
    // Sistema actual arreglo por chat gpt (sigue sin funcionar)
    Collision2D CheckCollision() {
        Collision2D collision2d;
        collision2d.Bool = false;
        collision2d.Object = nullptr;

        float anguloRad = this->Rotation.z * (M_PI / 180.0f);
        float c = std::abs(std::cos(anguloRad));
        float s = std::abs(std::sin(anguloRad));

        // Corregimos el tamaño de colisión para que coincida con el shader.
        // Como tu shader renderiza al doble (width*2) debido a la escala de 100,
        // usamos (Width * 2) y (Height * 2) como base para la envolvente.
        float colWidth = this->Width * 2.0f; 
        float colHeight = this->Height * 2.0f;

        float thisW_env = (colWidth * c) + (colHeight * s);
        float thisH_env = (colWidth * s) + (colHeight * c);

        // 3. Bordes desde el centro
        float thisMinX = this->Position.x - (thisW_env / 2.0f);
        float thisMaxX = this->Position.x + (thisW_env / 2.0f);
        float thisMinY = this->Position.y - (thisH_env / 2.0f);
        float thisMaxY = this->Position.y + (thisH_env / 2.0f);

        for (size_t i = 0; i < objetosReferenciados.size(); i++) {
            AnvilObject* otro = objetosReferenciados[i];

            if (otro != this) {
                float angRadOtro = otro->Rotation.z * (M_PI / 180.0f);
                float cO = std::abs(std::cos(angRadOtro));
                float sO = std::abs(std::sin(angRadOtro));
        
                float otroColW = otro->Width * 2.0f;
                float otroColH = otro->Height * 2.0f;

                float otroW_env = (otroColW * cO) + (otroColH * sO);
                float otroH_env = (otroColW * sO) + (otroColH * cO);

                float otroMinX = otro->Position.x - (otroW_env / 2.0f);
                float otroMaxX = otro->Position.x + (otroW_env / 2.0f);
                float otroMinY = otro->Position.y - (otroH_env / 2.0f);
                float otroMaxY = otro->Position.y + (otroH_env / 2.0f);

                bool check1 = (thisMaxX >= otroMinX && otroMaxX >= thisMinX);
                bool check2 = (thisMaxY >= otroMinY && otroMaxY >= thisMinY);

                if (check1 && check2) {
                    collision2d.Bool = true;
                    collision2d.Object = otro;
                    return collision2d; 
                }
            }
        }
    return collision2d;
    };

};



inline float deltaTime()
{
    return dontAskWhy;
}

//comandos internos para pasarnos parametros
//no se ni yo lo que hace

inline std::vector<std::vector<float>>& InternalPassVerticesList() {
    return cosasARenderizar;
}

inline std::vector<AnvilObject*>& InternalPassReferenceObjects() {
    return objetosReferenciados;
}

//delta time oculto para pasar desde el motor
inline void InternalPassDontAsk(float deltaaa)
{
    dontAskWhy = deltaaa;
};



