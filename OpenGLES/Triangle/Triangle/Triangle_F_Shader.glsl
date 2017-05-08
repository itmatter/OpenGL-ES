#version 300 es

precision mediump float;

in vec4 outVertexColor;

out vec4 outfragColor;

void main() {
   outfragColor = outVertexColor;
}
