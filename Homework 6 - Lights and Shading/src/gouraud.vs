#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float specularStrength;
uniform float diffuseMultiple;
uniform float shininess;

out vec3 result;

void main() {
  gl_Position = projection * view * model * vec4(aPos, 1.0);

  vec3 FragPos = vec3(model * vec4(aPos, 1.0));
  vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

  vec3 norm = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - FragPos);
  vec3 viewDirection = normalize(viewPos - FragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);

  vec3 ambient = ambientStrength * lightColor;

  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;

  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
  vec3 specular = specularStrength * spec * lightColor;

  result = ambient + diffuseMultiple * diffuse + specular;
}
