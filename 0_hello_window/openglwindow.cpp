#include "openglwindow.h"

OpenGLWindow::OpenGLWindow()
{

}

void OpenGLWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}
void OpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWindow::paintGL()
{
    glClearColor(0.7f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
