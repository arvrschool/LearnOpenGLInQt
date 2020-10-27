#ifndef MYOPENGLWINDOW_H
#define MYOPENGLWINDOW_H


#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QTime>

#include "myshader.h"
#include "mycamera.h"

class MyOpenGLWindow : public QOpenGLWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    MyOpenGLWindow();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent *event) override;    // 键盘事件
    void mouseMoveEvent(QMouseEvent *event) override; // 鼠标事件
    void wheelEvent(QWheelEvent *event) override;     // 滚轮事件

private:
    MyShader *basic_light_shader_;
    MyShader *light_cube_shader_;
    GLuint vao_;
    GLuint texture_;
    GLuint texture_0_;
    QTime time_;
    MyCamera *camera_;

    GLuint light_vao_;

    float last_frame_;
    float delta_time_;
    float fov_ = 0;
    bool first_mouse_ = true;
    float last_x_ = 1280.0f / 2.0f;
    float last_y_ = 960.0f / 2.0f;
};


#endif // MYOPENGLWINDOW_H
