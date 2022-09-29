#pragma once
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // Shader program ID
    unsigned int ID;

    // Constructor: must read and build shaders.
    Shader(const char* vertShaderPath, const char* fragShaderPath);
    void Use(); // To use and activate the shader

    // Utility uniform functions to set uniforms externally.
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    // Compile vertex and fragment shaders and returned a linked program ID
    void LinkShaderProgramID(const char* vertShaderSource, const char* fragShaderSource);
};