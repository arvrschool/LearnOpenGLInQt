#include "mainwindow.h"
#include "openglwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    OpenGLWindow *opengl_window = new OpenGLWindow;

    setCentralWidget(QWidget::createWindowContainer(opengl_window));
    resize(1280, 960);
}

MainWindow::~MainWindow()
{

}
