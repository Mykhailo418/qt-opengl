#include "OpenGLWidget.h"
#include "models/SimpleObject3D.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    resize(800, 600);
    pathToShaders = qApp->applicationDirPath() + "/shaders";
}
OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    setupShaders();


    initCube(1.0f);
    objects[0]->translate(QVector3D(-1.2, 0.0, 0.0));
    initCube(1.0f);
    objects[1]->translate(QVector3D(1.2, 0.0, 0.0));
}
void OpenGLWidget::resizeGL(int w, int h)
{
    float aspRatio = GLfloat(w) / (h ? GLfloat(h) : 1);
    matrixProjection.setToIdentity();
    matrixProjection.perspective(zoom, aspRatio, 0.01f, 100.0f);
}
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0, 0.0, zCameraValue);
    viewMatrix.rotate(rotation);

    defShaderProgram.bind();
    defShaderProgram.setUniformValue("u_projectionMatrix", matrixProjection);
    defShaderProgram.setUniformValue("u_viewMatrix", viewMatrix);
    defShaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    defShaderProgram.setUniformValue("u_lightPower", 1.0f);

    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw(&defShaderProgram, context()->functions());
    }
}

void OpenGLWidget::setupShaders()
{
    if (!defShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, pathToShaders + "/default.vsh")) {
        qDebug() << "Error loading default.vert shader\n";
        Q_ASSERT(nullptr);
    }
    if (!defShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, pathToShaders + "/default.fsh")) {
        qDebug() << "Error loading default.frag shader\n";
        Q_ASSERT(nullptr);
    }
    if (!defShaderProgram.link()) {
        qDebug() << "Error Linking Default Shader\n";
    }
}

void OpenGLWidget::initCube(float width)
{
    float halfWidth = width / 2;
    QVector<VertexData> verteces;
    VertexData vertex11 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f },QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex12 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 0.0f, 0.0f },QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex13 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 1.0f, 1.0f }, QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex14 = { QVector3D{ halfWidth, -halfWidth, halfWidth },QVector2D{ 1.0f, 0.0f }, QVector3D{ 0.0f, 0.0f, 1.0f } };

    VertexData vertex21 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex22 = { QVector3D{ halfWidth, -halfWidth, halfWidth }, QVector2D{ 0.0f, 0.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex23 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f, 1.0f },  QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex24 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f, 0.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };

    VertexData vertex31 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f }, QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex32 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 0.0f, 0.0f }, QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex33 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 1.0f, 1.0f },  QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex34 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f, 0.0f }, QVector3D{ 0.0f, 1.0f, 0.0f } };

    VertexData vertex41 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 0.0f, 1.0f }, QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex42 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 0.0f, 0.0f }, QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex43 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f, 1.0f },  QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex44 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f, 0.0f }, QVector3D{ 0.0f, 0.0f, -1.0f } };

    VertexData vertex51 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex52 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 0.0f, 0.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex53 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 1.0f, 1.0f },  QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex54 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f, 0.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };

    VertexData vertex61 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex62 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 0.0f, 0.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex63 = { QVector3D{ halfWidth, -halfWidth, halfWidth }, QVector2D{ 1.0f, 1.0f },  QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex64 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f, 0.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };


    verteces.append(vertex11);
    verteces.append(vertex12);
    verteces.append(vertex13);
    verteces.append(vertex14);
    verteces.append(vertex21);
    verteces.append(vertex22);
    verteces.append(vertex23);
    verteces.append(vertex24);
    verteces.append(vertex31);
    verteces.append(vertex32);
    verteces.append(vertex33);
    verteces.append(vertex34);
    verteces.append(vertex41);
    verteces.append(vertex42);
    verteces.append(vertex43);
    verteces.append(vertex44);
    verteces.append(vertex51);
    verteces.append(vertex52);
    verteces.append(vertex53);
    verteces.append(vertex54);
    verteces.append(vertex61);
    verteces.append(vertex62);
    verteces.append(vertex63);
    verteces.append(vertex64);

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    objects.append(new SimpleObject3D(verteces, indexes, QImage("E:/cube.png")));
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() == Qt::LeftButton) {
        mousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() != Qt::LeftButton) {
        return;
    }
    QVector2D diff = QVector2D(event->localPos()) - mousePosition;
    mousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0); // axis of rottion

    rotation = QQuaternion::fromAxisAndAngle(axis, angle) * rotation; // need * rotation in order to rotation calculate from previous rotation

    update(); // run update to update scene
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0) {
        zCameraValue += 0.25f;
    }
    else if (event->delta() < 0) {
        zCameraValue -= 0.25f;
    }

    update();
}