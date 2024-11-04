#version 410

layout(location = 0) in vec4 coord;
layout(location = 1) in vec3 normal;
layout(location = 3) in vec2 texcoord;

uniform mat4 Mv;
uniform mat4 Mn; 
uniform mat4 Mvp;

uniform vec4 lpos;
uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

out data {
  vec3 fragPos;
  vec3 fragNormal;
  vec3 fragLightDir;
} v;

void main(void) 
{
  vec3 veye = vec3(Mv*coord);
  vec3 light;
  if (lpos.w == 0) 
    light = normalize(vec3(lpos));
  else 
    light = normalize(vec3(lpos)-veye); 
  vec3 neye = normalize(vec3(Mn*vec4(normal,0.0f)));
  
  v.fragPos = -veye;
  v.fragNormal = neye;
  v.fragLightDir = light;  

  gl_Position = Mvp * coord;
}

