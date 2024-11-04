#version 330 core

layout (location = 0) in vec2 position;
uniform mat4 M;

void main() {
    gl_Position = M*vec4(position.x, position.y, 0.0, 1.0);
}
