#version 330 core

in vec2 uvs;

uniform sampler2D tex;
uniform vec3 color;
uniform float brightness;
uniform float contrast;

out vec4 fragColor;

void main() {
    vec4 pixcol = texture(tex, uvs);
    pixcol.rgb /= pixcol.a;

    // apply contrast
    pixcol.rgb *= contrast;

    // apply brightness
    pixcol.rgb += brightness;

    // return final pixel color.
    pixcol.rgb *= pixcol.a;

    fragColor = 0.25 * pixcol + 0.75 * pixcol * vec4(color, 1.0);
}
