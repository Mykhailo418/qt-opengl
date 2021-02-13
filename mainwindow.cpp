#include "mainwindow.h"
#include "OpenGLWidget.h"

MainWindow::MainWindow()
{
	resize(800, 600);
	OpenGLWidget* widget = new OpenGLWidget(this);
	setCentralWidget(widget);
}

MainWindow::~MainWindow()
{

}