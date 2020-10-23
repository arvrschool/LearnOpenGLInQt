#ifndef MYOPENGLWINDOW_H
#define MYOPENGLWINDOW_H


#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>

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
    GLuint texture_;
    GLuint texture_0_;
    QTime time_;
};


#endif // MYOPENGLWINDOW_H
