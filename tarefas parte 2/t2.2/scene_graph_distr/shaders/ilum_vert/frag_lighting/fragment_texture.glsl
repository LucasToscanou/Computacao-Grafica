#version 410
in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragLightDir;
in vec2 fragTexCoord;

uniform sampler2D textureSampler; // Texture sampler

uniform mat4 Mv;
uniform mat4 Mn; 
uniform mat4 Mvp;

uniform vec4 lpos;  // light pos in eye space
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

out vec4 fragColor;

void main(void) {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragLightDir);
    vec4 ambient = mamb * lamb;
    float ndotl = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = mdif * ldif * ndotl;

    // Texture color
    vec4 textureColor = texture(textureSampler, fragTexCoord);
    fragColor = (ambient + diffuse) * textureColor; // Combine lighting with texture
}
