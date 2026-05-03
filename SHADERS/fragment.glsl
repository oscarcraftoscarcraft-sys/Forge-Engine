#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform bool useTexture;

void main() {
    if (useTexture) {
        vec4 texColor = texture(ourTexture, TexCoord);
        FragColor = texColor * vec4(objectColor, 1.0);
    } else {
        FragColor = vec4(objectColor, 1.0);
    }
}