#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 proj;

void main()
{
  v_TexCoords = a_TexCoords;
  gl_Position = proj * vec4(a_Pos, 1.0);
}
