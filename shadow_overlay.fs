#version 330 core
out vec4 FragColor;

uniform float shadowIntensity; // Make sure this uniform is declared

void main() {
    FragColor = vec4(0.0, 0.0, 0.0, shadowIntensity);
}