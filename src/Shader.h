#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include <QOpenGLExtraFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

class Shader
{
public:
    Shader(QOpenGLExtraFunctions* functions, const char* vertexPath, const char* fragmentPath);

    Shader() = delete;
    ~Shader() = default;

    void use();

    void setInt(const std::string& name, int i);
    void setVec2(const std::string& name, const QVector2D& vec2);
    void setVec3(const std::string& name, const QVector3D& vec3);
    void setMat4(const std::string& name, const QMatrix4x4& mat4);

private:
    void checkCompileErrors(uint32_t shader, const std::string& type);

    uint32_t ID = 0;

    QOpenGLExtraFunctions* gl = nullptr;
};
