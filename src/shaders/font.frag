#version 330 core
layout (location = 0) out vec4 Color;

in vec2 v_TexCoords;

uniform sampler2D tex;

void main()
{
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, v_TexCoords).r);

  Color = sampled;
}
