#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent),
    texture(0), indexBuffer(QOpenGLBuffer::IndexBuffer)
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
}
void OpenGLWidget::resizeGL(int w, int h)
{
    float aspRatio = GLfloat(w) / (h ? GLfloat(h) : 1);
    matrixProjection.setToIdentity();
    matrixProjection.perspective(zoom, aspRatio, 0.1f, 10.0f);
}
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0.0, 0.0, -5.0);
    modelViewMatrix.rotate(30, 1.0, 0.0, 0.0);
    modelViewMatrix.rotate(30, 0.0, 1.0, 0.0);
    texture->bind(0);

    defShaderProgram.bind();
    defShaderProgram.setUniformValue("qt_ModelViewProjectionMatrix", matrixProjection * modelViewMatrix);
    defShaderProgram.setUniformValue("qt_Texture0", 0);

    arrBuffer.bind();

    int offset = 0;
    int vertLoc = defShaderProgram.attributeLocation("qt_Vertex");
    defShaderProgram.enableAttributeArray(vertLoc);
    defShaderProgram.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = defShaderProgram.attributeLocation("qt_MultiTexCoord0");
    defShaderProgram.enableAttributeArray(texLoc);
    defShaderProgram.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    indexBuffer.bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
}

static const char* vertexShaderSource =
"attribute highp vec4 qt_Vertex;\n"
"attribute highp vec2 qt_MultiTexCoord0;\n"
"uniform highp mat4 qt_ModelViewProjectionMatrix;\n"
"varying highp vec2 qt_TexCoord0;\n"
"void main(void)\n"
"{\n"
"   gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;\n"
"   qt_TexCoord0 = qt_MultiTexCoord0;\n"
"}";

static const char* fragmentShaderSource =
"uniform sampler2D qt_Texture0;\n"
"varying highp vec2 qt_TexCoord0;\n"
"void main(void)\n"
"{\n"
"   gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0);\n"
//"   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}";

void OpenGLWidget::setupShaders()
{
    if (!defShaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "Error loading default.vert shader\n";
        Q_ASSERT(nullptr);
    }
    if (!defShaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
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
    VertexData vertex11 = { QVector3D{ -halfWidth, halfWidth, halfWidth },QVector2D{ 0.0f, 1.0f },QVector3D{ 0.0f, 0.0f, 1.0f } };
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

    arrBuffer.create();
    arrBuffer.bind();
    arrBuffer.allocate(verteces.constData(), verteces.size() * sizeof(VertexData));
    arrBuffer.release();

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    indexBuffer.release();

    texture = new QOpenGLTexture(QImage("E:/cube.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}