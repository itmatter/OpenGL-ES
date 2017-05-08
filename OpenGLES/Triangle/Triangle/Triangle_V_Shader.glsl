#version 300 es

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inColor;

out vec4 outVertexColor;

void main() {
    gl_Position = inPosition ;
    outVertexColor = inColor;
}
