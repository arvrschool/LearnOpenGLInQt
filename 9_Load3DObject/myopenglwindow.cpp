#include "myopenglwindow.h"
#include <iostream>
#include <QKeyEvent>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SPECULAR 1

MyOpenGLWindow::MyOpenGLWindow()
{
    camera_ = new MyCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void MyOpenGLWindow::initializeGL()
{
    // 初始化 OpenGL函数，与QOpenGLFunctions_3_3_Core配合使用，之后便可以直接使用OpenGL函数
    initializeOpenGLFunctions();
    model_shader_ = new MyShader("F:/Projects/3DEngine/LearnOpenGLInQt/9_Load3DObject/Resources/shaders/model_loading_vert.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/9_Load3DObject/Resources/shaders/model_loading_fragment.txt");
    model_ = new Model("F:/Projects/3DEngine/LearnOpenGLInQt/9_Load3DObject/Resources/objects/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    time_.start();
}

void MyOpenGLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void MyOpenGLWindow::paintGL()
{
    GLfloat current_frame = (GLfloat)time_.elapsed()/100;
    delta_time_ = current_frame - last_frame_;
    last_frame_ = current_frame;
    MyShader shader("F:/Projects/3DEngine/LearnOpenGLInQt/9_Load3DObject/Resources/shaders/model_loading_vert.txt", "F:/Projects/3DEngine/LearnOpenGLInQt/9_Load3DObject/Resources/shaders/model_loading_fragment.txt");

    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    model_shader_->use();
    shader.use();
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera_->zoom_), (float)1280 / (float)960, 0.1f, 100.0f);
    glm::mat4 view = camera_->getViewMatrix();
//    model_shader_->setMat4("projection", projection);
//    model_shader_->setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
//    model_shader_->setMat4("model", model);
//    model_->draw(*model_shader_);
    shader.setMat4("model", model);
    model_->draw(shader);

    update();
}

void MyOpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    float move_speed = 2.5 * delta_time_;
    if(event->key() == Qt::Key_W){
        camera_->processKeyboard(Camera_Movement::FORWARD, move_speed);
    }
    if(event->key() == Qt::Key_S){
        camera_->processKeyboard(Camera_Movement::BACKWARD, move_speed);
    }
    if(event->key() == Qt::Key_A){
        camera_->processKeyboard(Camera_Movement::LEFT, move_speed);
    }
    if(event->key() == Qt::Key_D){
        camera_->processKeyboard(Camera_Movement::RIGHT, move_speed);
    }
    if(event->key() == Qt::Key_E){ //上升
        camera_->processKeyboard(Camera_Movement::UP, move_speed);
    }
    if(event->key() == Qt::Key_Q){ //下降
        camera_->processKeyboard(Camera_Movement::DOWN, move_speed);
    }
}

void MyOpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
    float x_pos = event->pos().x();
    float y_pos = event->pos().y();
    if (first_mouse_)
    {
        last_x_ = x_pos;
        last_y_ = y_pos;
        first_mouse_ = false;
    }

    float xoffset = x_pos - last_x_;
    float yoffset = last_y_ - y_pos; // reversed since y-coordinates go from bottom to top

    last_x_ = x_pos;
    last_y_ = y_pos;

    camera_->processMouseMovement(xoffset, yoffset);
}

void MyOpenGLWindow::wheelEvent(QWheelEvent *event)
{
    std::cout << "wheel event" << std::endl;
    QPoint offset = event->angleDelta();
    if(fov_ >= 1.0f && fov_ <=45.0f)
        fov_ -= ((GLfloat)offset.y())/20;
    if(fov_ < 1.0f)
        fov_ = 1.0f;
    if(fov_ > 45.0f)
        fov_ = 45.0f;
    camera_->processMouseScroll(fov_);
}
