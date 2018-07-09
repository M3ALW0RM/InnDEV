#version 450 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexUV;
uniform mat4 model;
uniform mat4 viewPerspective;
out vec2 UV;
out vec3 normal;
void main(){
  gl_Position = viewPerspective * model * vec4(vertexPosition_modelspace, 1.0f);
  UV = vertexUV;
  normal = (inverse(transpose(model)) * vec4(vertexNormal, 0.f)).xyz;
}