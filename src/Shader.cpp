#include "Shader.h"

Shader::Shader(QOpenGLExtraFunctions* functions, const char* vertexPath, const char* fragmentPath)
    : gl(functions)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit);
    fShaderFile.exceptions(std::ifstream::failbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into GLchar array
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    uint32_t vertex, fragment;
    // Vertex shader
    vertex = gl->glCreateShader(GL_VERTEX_SHADER);
    gl->glShaderSource(vertex, 1, &vShaderCode, nullptr);
    gl->glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment shader
    fragment = gl->glCreateShader(GL_FRAGMENT_SHADER);
    gl->glShaderSource(fragment, 1, &fShaderCode, nullptr);
    gl->glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader program
    ID = gl->glCreateProgram();
    gl->glAttachShader(ID, vertex);
    gl->glAttachShader(ID, fragment);
    gl->glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    gl->glDeleteShader(vertex);
    gl->glDeleteShader(fragment);
}

void Shader::use() { gl->glUseProgram(ID); }

void Shader::setInt(const std::string &name, int i) {
  gl->glUniform1i(gl->glGetUniformLocation(ID, name.c_str()), i);
}
void Shader::setVec2(const std::string &name, const QVector2D &vec2) {
  gl->glUniform2f(gl->glGetUniformLocation(ID, name.c_str()), vec2.x(), vec2.y());
}
void Shader::setVec3(const std::string &name, const QVector3D &vec3) {
  gl->glUniform3f(gl->glGetUniformLocation(ID, name.c_str()), vec3.x(), vec3.y(), vec3.z());
}
void Shader::setMat4(const std::string &name, const QMatrix4x4 &mat4) {
  gl->glUniformMatrix4fv(gl->glGetUniformLocation(ID, name.c_str()), 1, false, mat4.data());
}

void Shader::checkCompileErrors(uint32_t shader, const std::string& type) {
    int success {0};
    char infoLog[1024];
    if (type != "PROGRAM") {
        gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            gl->glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR:SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------------- --" << std::endl;
        }
    }
    else {
        gl->glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            gl->glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR:PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------------------------- --" << std::endl;
        }
    }
}
