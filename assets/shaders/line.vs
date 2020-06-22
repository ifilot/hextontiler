#version 330 core

in vec2 position;

uniform mat4 mvp;

void main() {
    // output position of the vertex
    gl_Position = mvp * vec4(position, 1.0, 1.0);
}
