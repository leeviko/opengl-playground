#include "Shader.hpp"

#include <iostream>

Shader &Shader::Use()
{
  glUseProgram(m_ShaderId);
  return *this;
}

GLint Shader::GetUniformLocation(const std::string &name) const
{

  auto it = UniformLocationCache.find(name);
  if (it != UniformLocationCache.end())
    return it->second;

  UniformLocationCache[name] = glGetUniformLocation(m_ShaderId, name.c_str());

  return UniformLocationCache[name];
}

void Shader::Compile(const char *vertexSource, const char *fragmentSource)
{
  unsigned int sVertex, sFragment;

  sVertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(sVertex, 1, &vertexSource, NULL);
  glCompileShader(sVertex);
  CheckCompileErrors(sVertex, "VERTEX");

  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(sFragment, 1, &fragmentSource, NULL);
  glCompileShader(sFragment);
  CheckCompileErrors(sFragment, "FRAGMENT");

  m_ShaderId = glCreateProgram();
  glAttachShader(m_ShaderId, sVertex);
  glAttachShader(m_ShaderId, sFragment);
  glLinkProgram(m_ShaderId);
  CheckCompileErrors(m_ShaderId, "PROGRAM");

  glDeleteShader(sVertex);
  glDeleteShader(sFragment);
}

void Shader::SetFloat(const char *name, float value)
{
  glUniform1f(GetUniformLocation(name), value);
}
void Shader::SetInteger(const char *name, int value)
{
  glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetIntegerv(const char *name, int value[])
{
  glUniform1iv(GetUniformLocation(name), sizeof(value) / sizeof(value[0]), value);
}
void Shader::SetVector2f(const char *name, float x, float y)
{
  glUniform2f(GetUniformLocation(name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value)
{
  glUniform2f(GetUniformLocation(name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z)
{
  glUniform3f(GetUniformLocation(name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value)
{
  glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w)
{
  glUniform4f(GetUniformLocation(name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value)
{
  glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(unsigned int object, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
    }
  }
  else
  {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
    }
  }
}
