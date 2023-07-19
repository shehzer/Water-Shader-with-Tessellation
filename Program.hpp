#ifndef PROGRAM_HPP
#define PROGRAM_HPP 1
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program
{
  unsigned programId = 0;
  unsigned * amount;

  unsigned getUniformId(const char * name) const;

  void clear();

public:
  Program();
  void create();
  void link() const;
  void attachShader(const Shader & shader) const;

  void use() const {glUseProgram(programId);}

  void setInt(const char * name, int i) const;
  void setFloat(const char * name, float f) const;
  void setVec2(const char * name, const glm::vec2 & vec) const;
  void setVec3(const char * name, const glm::vec3 & vec) const;
  void setMat4(const char * name, const glm::mat4 & mat) const;

  unsigned getId() const {return programId;}
};
#endif
