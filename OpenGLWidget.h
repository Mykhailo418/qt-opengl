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

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = nullptr);
    ~OpenGLWidget();

    float zoom = 45.0f;

public slots:

private:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void setupShaders();
    void initCube(float width);

    QString pathToShaders;
    QMatrix4x4 matrixProjection;
    QOpenGLShaderProgram defShaderProgram;
    QOpenGLTexture* texture;
    QOpenGLBuffer arrBuffer;
    QOpenGLBuffer indexBuffer;

};

#endif // OPENGLWIDGET_H