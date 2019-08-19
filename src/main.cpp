#include <iostream>
#include <vector>

#include <QOpenGLWindow>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QTimer>

//Camera camera(cameraPos, cameraUp);
//GLfloat lastX = WIDTH / 2.0;
//GLfloat lastY = HEIGHT / 2.0;
//bool keys[1024];

#include "TestOpenGLWindow.h"

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 3.3 core context");
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    else {
        qDebug("Requesting 3.0 ES context");
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);

    TestOpenGLWindow window {800, 600};
    window.show();

    return app.exec();
}

//void do_movement(GLfloat deltaTime)
//{
	//GLfloat cameraSpeed = 5.0f * deltaTime;
	//if (keys[GLFW_KEY_W])
		//camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	//if (keys[GLFW_KEY_S])
		//camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	//if (keys[GLFW_KEY_A])
		//camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	//if (keys[GLFW_KEY_D])
		//camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
//}

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        //glfwSetWindowShouldClose(window, GL_TRUE);
	//if (action == GLFW_PRESS)
		//keys[key] = true;
	//if (action == GLFW_RELEASE)
		//keys[key] = false;
//}

//bool firstMouse = true;
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
	//if (firstMouse)
	//{
		//lastX = xpos;
		//lastY = ypos;
		//firstMouse = false;
	//}

	//GLfloat xoffset = xpos - lastX;
	//GLfloat yoffset = lastY - ypos;
	//lastX = xpos;
	//lastY = ypos;
	//camera.ProcessMouseMovement(xoffset, yoffset);
//}

//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
	//camera.ProcessMouseScroll(yoffset);
//}
