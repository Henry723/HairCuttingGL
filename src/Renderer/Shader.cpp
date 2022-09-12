#include "Shader.h"

Shader::Shader(const char* vertShaderPath, const char* fragShaderPath)
{
    // 1. Retrieve the source code from given file path
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Poll ifstream to ensure they can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // a. Open files
        vShaderFile.open(vertShaderPath);
        fShaderFile.open(fragShaderPath);
        std::stringstream vShaderStream, fShaderStream;

        // b. Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // c. Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // d. Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile the shaders
    linkShaderProgramID(vShaderCode, fShaderCode);
}

void Shader::use()
{
    glUseProgram(ID);
}

// Utility uniform functions to set uniforms externally.
//------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

//------------------------------------
// Compile vertex and fragment shaders and returned a linked program ID.
void Shader::linkShaderProgramID(const char* vertShaderSource, const char* fragShaderSource)
{
    // Vertex shader compilation.
    //-------------------------------
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource first param takes the shader object to compile. 
    // Second param specifies the number of strings from the source code we are passing.
    // Third param is the source code of the vertex shader.
    glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertexShader);

    // We are compiling at runtime, it is beneficial to double check the compilation was a success.
    int success;
    char infoLog[512];
    // Check for shader compile errors.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Fragment shader compilation.
    //-------------------------------
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return;
    }

    // Create program by linking shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return;
    }
    // Delete the vertex and fragment shader since we linked into the program object.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}