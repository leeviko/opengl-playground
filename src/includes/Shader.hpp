#pragma once
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "MyMath.hpp"
#include <string>

class Shader
{
private:
  mutable std::unordered_map<std::string, GLint> UniformLocationCache;

public:
  unsigned int m_ShaderId;

  Shader() {}

  Shader &Use();

  void Compile(const char *vertexSource, const char *fragmentSource);

  void SetFloat(const char *name, float value);
  void SetInteger(const char *name, int value);
  void SetIntegerv(const char *name, int value[]);
  void SetVector2f(const char *name, float x, float y);
  void SetVector2f(const char *name, const glm::vec2 &value);
  void SetVector3f(const char *name, float x, float y, float z);
  void SetVector3f(const char *name, const glm::vec3 &value);
  void SetVector4f(const char *name, float x, float y, float z, float w);
  void SetVector4f(const char *name, const glm::vec4 &value);
  void SetMatrix4(const char *name, const glm::mat4 &matrix);

private:
  GLint GetUniformLocation(const std::string &name) const;
  void CheckCompileErrors(unsigned int object, std::string type);
};
