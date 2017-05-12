#version 300 es

precision mediump float;

uniform sampler2D colorMap;

in vec2 varyTextCoord;
in vec4 v_color;

out vec4 outColor;

void main() {
    outColor = texture(colorMap, varyTextCoord) * v_color;
}
