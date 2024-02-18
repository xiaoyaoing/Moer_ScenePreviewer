#pragma once
#include <glad/glad.h>

#include <Eigen/Dense>
#include <string>


using Vector4f = Eigen::Vector4f;
using Vector3f = Eigen::Vector3f;
using Matrix3f = Eigen::Matrix3f;
using Matrix4f = Eigen::Matrix4f;

extern char phonefs[];
extern char phonevs[];
extern char screenfs[];
extern char screenvs[];

// Reference: Learn OpenGL (Joey de Vries)
class Shader {
  public:
   unsigned int programID;  // Shader program id
   // constructor reads and builds the shader
   Shader() = default;
   Shader(const char *vertexPath, const char *fragmentPath);
   // use/activate the shader
   void use();
   // utility uniform functions
   void setBool(const std::string &name, bool value) const;
   void setInt(const std::string &name, int value) const;
   void setFloat(const std::string &name, float value) const;
   void setVector3f(const std::string &name, Vector3f value) const;
   void setVector4f(const std::string &name, Vector4f value) const;
   void setMatrix4f(const std::string &name, Matrix4f value) const;

  protected:
   void loadShaderFromPath(const char *path, std::string &code);
   unsigned int compileShader(std::string &shaderCode, GLenum shaderType);
   void createProgram(unsigned int vertex, unsigned int fragment);
};

class ScreenShader : public Shader {
  public:
   ScreenShader();
};

class PhoneShader : public Shader {
  public:
   PhoneShader();
};