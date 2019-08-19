#include "TestOpenGLWindow.h"

#include <QOpenGLFunctions>

#include "Shader.h"

#include <QTimer>

TestOpenGLWindow::TestOpenGLWindow(int width/* = 800*/, int height/* = 600*/) {
    setMinimumSize({width, height});
}

TestOpenGLWindow::~TestOpenGLWindow() {}

void TestOpenGLWindow::initializeGL() {
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glEnable(GL_DEPTH_TEST);

    m_Program.reset(new Shader(gl, "resources/shaders/model_loading_es.vert", "resources/shaders/model_loading_es.frag"));
    m_Nanosuit.load(gl, "resources/nanosuit.obj");

    m_ModelMatrix.translate(0.0f, -1.75f, 0.0f);
    m_ModelMatrix.scale(0.2f);

    m_LastFrame = std::chrono::steady_clock::now();

    m_Camera.Position = QVector3D{0.0f, 0.0f,  3.0f};
    m_Camera.WorldUp = QVector3D{0.0f, 1.0f,  0.0f};

    QTimer* timer = new QTimer{this};
    connect(timer, &QTimer::timeout, this, static_cast<void (QOpenGLWindow::*)(void)>(&QOpenGLWindow::update));
    timer->start(1000/60.0);
}

void TestOpenGLWindow::resizeGL(int w, int h) {
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glViewport(0, 0, width(), height());
}

void TestOpenGLWindow::paintGL() {
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    auto currentFrame = std::chrono::steady_clock::now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - m_LastFrame).count();
    m_LastFrame = currentFrame;

    //do_movement(deltaTime);

    gl->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Program->use();

    // Transformation matrices
    QMatrix4x4 projection {};
    projection.perspective(m_Camera.Zoom, (float)width()/(float)height(), 0.1f, 100.0f);
    QMatrix4x4 view = m_Camera.GetViewMatrix();

    m_Program->setMat4("projection", projection);
    m_Program->setMat4("view", view);

    // Draw the loaded model
    m_Program->setMat4("model", m_ModelMatrix);
    m_Nanosuit.draw(gl, *m_Program);
}

void TestOpenGLWindow::mouseMoveEvent(QMouseEvent* pEvent)
{

}

void TestOpenGLWindow::keyPressEvent(QKeyEvent* pEvent)
{

}

void TestOpenGLWindow::keyReleaseEvent(QKeyEvent* pEvent)
{

}
