#version 410

in data {
  vec3 fragPos;
  vec3 fragNormal;
  vec3 fragLightDir;
} f;

uniform vec4 lamb;
uniform vec4 ldif;
uniform vec4 lspe;

uniform vec4 mamb;
uniform vec4 mdif;
uniform vec4 mspe;
uniform float mshi;

out vec4 fragColor;

void main(void) 
{
  vec3 norm = normalize(f.fragNormal);
  vec3 lightDir = normalize(f.fragLightDir);
  
  // Ambient component
  vec4 ambient = mamb * lamb;
  
  // Diffuse component
  float ndotl = max(dot(norm, lightDir), 0.0);
  vec4 diffuse = mdif * ldif * ndotl;
  
  // Specular component
  vec4 specular = vec4(0.0);
  if (ndotl > 0.0) {
      vec3 viewDir = normalize(-f.fragPos);
      vec3 reflectDir = reflect(-lightDir, norm);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), mshi);
      specular = mspe * lspe * spec;
  }

  // Final color
  fragColor = ambient + diffuse + specular;
  
}
