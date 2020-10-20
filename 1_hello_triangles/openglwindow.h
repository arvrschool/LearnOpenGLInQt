#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

class OpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    OpenGLWindow();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    int createShader(GLenum type, const char* source);
    GLuint createProgram(int vertex, int fragment);
};

#endif // OPENGLWINDOW_H
