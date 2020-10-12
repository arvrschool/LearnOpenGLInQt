#include "mainwindow.h"
#include "openglwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    OpenGLWindow *openglWindow = new OpenGLWindow;

    setCentralWidget(QWidget::createWindowContainer(openglWindow));
    resize(1280, 960);

}

MainWindow::~MainWindow()
{

}
