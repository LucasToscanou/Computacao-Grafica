#version 330 core

layout (location = 0) in vec2 position;
layout(location = 1) in vec2 aTexCoord; // Vertex texture coordinates (u, v)

out vec2 TexCoord; 

uniform mat4 Mvp;

void main() {
    gl_Position = Mvp*vec4(position.x, position.y, 0.0, 1.0);
    TexCoord = aTexCoord; // Pass texture coordinates to the fragment shader
}