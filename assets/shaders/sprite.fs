#version 330 core

in vec2 uvs;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main() {
    fragColor = 0.25 * texture(tex, uvs) + 0.75 * texture(tex, uvs) * vec4(color, 1.0);
}
