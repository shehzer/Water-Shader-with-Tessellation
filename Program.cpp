#include "Program.hpp"
/**  Helper Functions to set attributes  **/
Program::Program()
{
  amount = new unsigned;
  *amount = 1;
}

void Program::clear()
{
  *amount = *amount - 1;
  if (*amount == 0)
  {
    delete amount;
    if (programId != 0)
      glDeleteProgram(programId);
  }
}

void Program::create()
{
  programId = glCreateProgram();
}

void Program::attachShader(const Shader & shader) const
{
  glAttachShader(programId, shader.getId());
  shader.clear();
}

void Program::link() const
{
  glLinkProgram(programId);
  int success;
  glGetProgramiv(programId, GL_LINK_STATUS, &success);
}

unsigned Program::getUniformId(const char * name) const
{
  return glGetUniformLocation(programId, name);
}

void Program::setInt(const char * name, int i) const
{
  glUniform1i(getUniformId(name), i);
}

void Program::setFloat(const char * name, float f) const
{
  glUniform1f(getUniformId(name), f);
}

void Program::setVec2(const char * name, const glm::vec2 & vec) const
{
  glUniform2fv(getUniformId(name), 1, glm::value_ptr(vec));
}

void Program::setVec3(const char * name, const glm::vec3 & vec) const
{
  glUniform3fv(getUniformId(name), 1, glm::value_ptr(vec));
}

void Program::setMat4(const char * name, const glm::mat4 & mat) const
{
  glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, glm::value_ptr(mat));
}
