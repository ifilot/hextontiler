#version 330 core

in vec2 position;
in vec2 uv;

out vec2 uvs;

uniform mat4 mvp;

void main() {
    // output position of the vertex
    gl_Position = mvp * vec4(position, 0.0, 1.0);

    // output uv position
    uvs = uv;
}
