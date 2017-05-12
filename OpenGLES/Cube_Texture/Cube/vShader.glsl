#version 300 es

uniform mat4 u_mvpMatrix;

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textCoordinate;

uniform sampler2D s_texture;                      

out vec4 v_color;
out vec2 varyTextCoord;


void main() {
    varyTextCoord = a_textCoordinate;
    v_color = a_color;
    gl_Position = u_mvpMatrix * a_position;
}                                           
