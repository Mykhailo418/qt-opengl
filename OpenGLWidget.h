#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QSurfaceFormat>
#include <QtOpenGL>
#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

#include "models/VertexData.h"

class SimpleObject3D;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

public slots:

private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void setupShaders();
    void initCube(float width);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

    float zoom = 45.0f;
    float zCameraValue = -5.0f;

    QString pathToShaders;
    QMatrix4x4 matrixProjection;
    QOpenGLShaderProgram defShaderProgram;
    QVector2D mousePosition;
    QQuaternion rotation;
    QVector<SimpleObject3D*> objects;
};

#endif // OPENGLWIDGET_H