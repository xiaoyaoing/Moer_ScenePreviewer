#pragma once
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"


class Shader {
  public:
   unsigned int programID;
   // constructor reads and builds the shader
   Shader(const char *vertexPath, const char *fragmentPath);
   // use/activate the shader
   void use();
   // utility uniform functions
   void setBool(const std::string &name, bool value) const;
   void setInt(const std::string &name, int value) const;
   void setFloat(const std::string &name, float value) const;
};
