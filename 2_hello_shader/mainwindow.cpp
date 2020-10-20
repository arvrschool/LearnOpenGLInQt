#include "mainwindow.h"
#include "myopenglwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MyOpenGLWindow *opengl_window = new MyOpenGLWindow;

    setCentralWidget(QWidget::createWindowContainer(opengl_window));
    resize(1280, 960);
}

MainWindow::~MainWindow()
{

}
