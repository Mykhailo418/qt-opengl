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
#include <QBasicTimer>

#include "models/VertexData.h"

class SimpleObject3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;
class ObjectEngine3D;

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
    void generateCubes();
    GLenum getDrawMode(int vertecesCount);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);

    float zoom = 45.0f;
    float angleObject = 0.0f;

    QString pathToShaders;
    QMatrix4x4 matrixProjection;
    QOpenGLShaderProgram defShaderProgram;
    QOpenGLShaderProgram skyboxShaderProgram;
    QVector2D mousePosition;
    QVector<ObjectEngine3D*> objects;
    QVector<Transformational*> transformObjects;
    QVector<Group3D*> groups;
    QBasicTimer timer;
    Camera3D* camera;
    SkyBox* skybox;
};

#endif // OPENGLWIDGET_H