#version 410
layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 Mv;
uniform mat4 Mn;
uniform mat4 Mvp;
uniform vec4 lpos;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragLightDir;
out vec2 fragTexCoord;

void main(void) {
    vec3 veye = vec3(Mv * coord);
    fragPos = veye;
    fragNormal = normalize(mat3(Mn) * normal);
    fragLightDir = normalize(vec3(lpos) - veye);
    fragTexCoord = texcoord; // Pass texture coordinates
    gl_Position = Mvp * coord;
}
