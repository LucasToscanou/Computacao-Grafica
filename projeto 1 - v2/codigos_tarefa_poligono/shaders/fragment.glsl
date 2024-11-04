#version 330 core

in vec2 st;
uniform vec4 objectColor;
uniform sampler2D face;

out vec4 FragColor;

void main()
{
    vec4 texel = texture(face, st);
    FragColor = texel;
    
}
