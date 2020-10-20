#ifndef MYOPENGLWINDOW_H
#define MYOPENGLWINDOW_H


#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>

#include "myshader.h"

class MyOpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    MyOpenGLWindow();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    MyShader *shader_;
    GLuint vao_;
};


#endif // MYOPENGLWINDOW_H
