#pragma once

#include <QOpenGLWindow>

#include "Model.h"
#include "Camera.h"
#include "Shader.h"

#include <memory>
#include <chrono>

class TestOpenGLWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    TestOpenGLWindow(int width = 800, int height = 600);
    ~TestOpenGLWindow();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* pEvent) override;

    void keyPressEvent(QKeyEvent* pEvent) override;
    void keyReleaseEvent(QKeyEvent* pEvent) override;

private:
    std::chrono::steady_clock::time_point m_LastFrame;

    std::unique_ptr<Shader> m_Program;

    Model m_Nanosuit;
    Camera m_Camera;

    QMatrix4x4 m_ModelMatrix;
};
