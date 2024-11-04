#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
uniform mat4 Mvp;

out vec2 st;

void main() {
    st = texCoord;
    gl_Position = Mvp*vec4(position.x, position.y, 0.0, 1.0);
}
