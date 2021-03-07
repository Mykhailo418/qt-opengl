#include <iostream>
#include "OpenGLWidget.h"
#include "interfaces/transformational.h"
#include "models/SimpleObject3D.h"
#include "models/ObjectEngine3D.h"
#include "models/Group3D.h"
#include "models/Camera3D.h"
#include "models/SkyBox.h"
#include "interfaces/IndexesMode.h"
#include "models/Material.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    resize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
    pathToShaders = qApp->applicationDirPath() + "/shaders";
    camera = new Camera3D;
    camera->translate(QVector3D(0.0f, 0.0f, -5.0f));
}
OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();

    /*delete camera;
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    for (int j = 0; j < transformObjects.size(); j++) {
        delete transformObjects[j];
    }
    for (int k = 0; k < groups.size(); k++) {
        delete groups[k];
    }*/

    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    setupShaders();

    //generateCubes();
    objects.append(new ObjectEngine3D);
    //objects[objects.size() - 1]->loadObjectFromFile("E:/SweetDreams/3dModels/obj/85-cottage_obj/cottage_obj.obj");
    //objects[objects.size() - 1]->loadObjectFromFile("E:/SweetDreams/3dModels/obj/female/female_3dmax.obj");
    //objects[objects.size() - 1]->loadObjectFromFile("E:/SweetDreams/dev/svn/trunk/3rdparty/QT-3D-Model-Viewer/QT3DModelViewer/Models/Patrick/Patrick.obj");
    //objects[objects.size() - 1]->loadObjectFromFile("E:/SweetDreams/3dModels/obj/castle/Castle OBJ.obj");
    objects[objects.size() - 1]->loadObjectFromFile("E:/SweetDreams/3dModels/fbx/45-cottage/Cottage_FREE2.obj");
    transformObjects.append(objects[objects.size() - 1]);
    skybox = new SkyBox(2000, QImage("E:/Downloads/skybox.png"));
    timer.start(30, this);
}
void OpenGLWidget::resizeGL(int w, int h)
{
    float aspRatio = GLfloat(w) / (h ? GLfloat(h) : 1);
    matrixProjection.setToIdentity();
    matrixProjection.perspective(zoom, aspRatio, 0.01f, 3000.0f);
}
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skyboxShaderProgram.bind();
    skyboxShaderProgram.setUniformValue("u_projectionMatrix", matrixProjection);

    camera->draw(&skyboxShaderProgram);
    skybox->draw(&skyboxShaderProgram, context()->functions());
    skyboxShaderProgram.release();

    defShaderProgram.bind();
    defShaderProgram.setUniformValue("u_projectionMatrix", matrixProjection);
    defShaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    defShaderProgram.setUniformValue("u_lightPower", 2.0f);

    camera->draw(&defShaderProgram);

    for (int i = 0; i < transformObjects.size(); i++) {
        transformObjects[i]->draw(&defShaderProgram, context()->functions());
    }
    defShaderProgram.release();
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

    if (!skyboxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, pathToShaders + "/skybox.vsh")) {
        qDebug() << "Error loading default.vert shader\n";
        Q_ASSERT(nullptr);
    }
    if (!skyboxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, pathToShaders + "/skybox.fsh")) {
        qDebug() << "Error loading default.frag shader\n";
        Q_ASSERT(nullptr);
    }
    if (!skyboxShaderProgram.link()) {
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

    Material* newMtl = new Material;
    newMtl->setDiffuseMap("E:/cube.png");
    newMtl->setShinnes(96);
    newMtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));

    /*ObjectEngine3D* objectEngine = new ObjectEngine3D;
    objectEngine->addObject(new SimpleObject3D(verteces, indexes, newMtl));

    objects.append(objectEngine);*/
}

void OpenGLWidget::generateCubes()
{
    float step = 2.0f;
    groups.append(new Group3D);
 
    for (float x = -step; x <= step; x += step) {
        for (float y = -step; y <= step; y += step) {
            for (float z = -step; z <= step; z += step) {
                initCube(1.0f);
                objects[objects.size() - 1]->translate(QVector3D(x, y, z));
                groups[groups.size() - 1]->addObject(objects[objects.size() - 1]);
            }
        }
    }
    groups[0]->translate(QVector3D(-4.0f, 0.0f, 0.0f));

    groups.append(new Group3D);
    for (float x = -step; x <= step; x += step) {
        for (float y = -step; y <= step; y += step) {
            for (float z = -step; z <= step; z += step) {
                initCube(1.0f);
                objects[objects.size() - 1]->translate(QVector3D(x, y, z));
                groups[groups.size() - 1]->addObject(objects[objects.size() - 1]);
            }
        }
    }
    groups[1]->translate(QVector3D(4.0f, 0.0f, 0.0f));

    groups.append(new Group3D);
    groups[2]->addObject(groups[0]);
    groups[2]->addObject(groups[1]);
    transformObjects.append(groups[2]);
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

    camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));

    update(); // run update to update scene
}

void OpenGLWidget::wheelEvent(QWheelEvent* event)
{
    float step = 5.25f;
    if (event->delta() > 0) {
        camera->translate(QVector3D(0.0f, 0.0f, step));
    }
    else if (event->delta() < 0) {
        camera->translate(QVector3D(0.0f, 0.0f, -step));
    }

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        qDebug() << "left\n";
        break;
    case Qt::Key_Right:
        qDebug() << "right\n";
        break;
    case Qt::Key_Up:
        qDebug() << "up\n";
        break;
    case Qt::Key_Down:
        qDebug() << "down\n";
        break;
    }
    update();
}

void OpenGLWidget::timerEvent(QTimerEvent* event)
{
    for (int i = 0; i < objects.size() - 1; i++) {
        if (i % 2 == 0) {
            objects[i]->rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0, 0.0), qSin(angleObject)));
        }
        else {
            objects[i]->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0, 0.0), qCos(angleObject)));
        }
    }
    angleObject += M_PI / 100.0f;
    update();
}


GLenum OpenGLWidget::getDrawMode(int vertecesCount)
{
    if (vertecesCount == 3) {
        return GL_TRIANGLES;
    }
    else if (vertecesCount == 4) {
        return GL_QUADS;
    }
    return GL_POLYGON;
}