#version 330 core

//uniform vec3 color;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, texCoord);
}