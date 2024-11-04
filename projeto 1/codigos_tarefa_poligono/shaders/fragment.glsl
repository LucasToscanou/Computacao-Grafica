#version 330 core

in vec2 TexCoord; // Interpolated texture coordinates from the vertex shader

out vec4 FragColor;

uniform sampler2D texture1; // The texture sampler
uniform vec3 objectColor; // The base color of the object

void main()
{
    // Sample the texture at the given coordinates
    vec4 texColor = texture(texture1, TexCoord);

    // Mix the texture color with the object color
    FragColor = texColor * vec4(objectColor, 1.0);
}
