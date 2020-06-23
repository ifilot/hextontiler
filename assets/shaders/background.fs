#version 330 core

in vec2 uvs;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main() {
    fragColor = texture(tex, uvs).a * vec4(color, texture(tex, uvs).a);
}
