#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
   // 1. retrieve the vertex/fragment source code from filePath
   std::string vertexCode;
   std::string fragmentCode;

   loadShaderFromPath(vertexPath, vertexCode);
   loadShaderFromPath(fragmentPath, fragmentCode);

   // 2. compile shaders
   unsigned int vertex = 0, fragment = 0;
   vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
   fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

   createProgram(vertex, fragment);
   // delete shaders; they’re linked into our program and no longer necessary
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(programID); }

void Shader::setBool(const std::string& name, bool value) const {
   glUniform1i(glGetUniformLocation(programID, name.c_str()),
               static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
   glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
   glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVector3f(const std::string& name, Vector3f value) const {
   glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x(),
               value.y(), value.z());
}

void Shader::setVector4f(const std::string& name, Vector4f value) const {
   glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x(),
               value.y(), value.z(), value.w());
}

void Shader::setMatrix4f(const std::string& name, Matrix4f value) const {
   glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1,
                      GL_FALSE, value.data());
}

void Shader::loadShaderFromPath(const char* path, std::string& code) {
   std::ifstream shaderFile;
   shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try {
      // open files
      shaderFile.open(path);
      std::stringstream shaderStream;
      // read file’s buffer contents into streams
      shaderStream << shaderFile.rdbuf();
      // close file handlers
      shaderFile.close();
      // convert stream into string
      code = shaderStream.str();
   } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
   }
}

unsigned int Shader::compileShader(std::string& shaderCode, GLenum shaderType) {
   int success = 0;
   char infoLog[512];
   unsigned int shader;
   const char* charShaderCode = shaderCode.c_str();

   shader = glCreateShader(shaderType);
   glShaderSource(shader, 1, &charShaderCode, NULL);
   glCompileShader(shader);
   // print compile errors if any
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
   };
   return shader;
}

void Shader::createProgram(unsigned int vertex, unsigned int fragment) {
   int success = 0;
   char infoLog[512];
   programID = glCreateProgram();
   glAttachShader(programID, vertex);
   glAttachShader(programID, fragment);
   glLinkProgram(programID);
   // print linking errors if any
   glGetProgramiv(programID, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(programID, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
   }
}

ScreenShader::ScreenShader() : Shader() {
   std::string vertexCode(screenvs);
   std::string fragmentCode(screenfs);
   // 2. compile shaders
   unsigned int vertex = 0, fragment = 0;
   vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
   fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

   createProgram(vertex, fragment);
   // delete shaders; they’re linked into our program and no longer necessary
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}

PhoneShader::PhoneShader() : Shader() {
   std::string vertexCode(phonevs);
   std::string fragmentCode(phonefs);
   // 2. compile shaders
   unsigned int vertex = 0, fragment = 0;
   vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
   fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

   createProgram(vertex, fragment);
   // delete shaders; they’re linked into our program and no longer necessary
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}