#include "SkyBox.h"
#include "SimpleObject3D.h"
#include "Material.h"

SkyBox::SkyBox(float width, QImage &textureImg)
{
    initCube(width, textureImg);
}
SkyBox::~SkyBox()
{

}

void SkyBox::rotate(const QQuaternion& r)
{
	(void)r;
}
void SkyBox::translate(const QVector3D& t)
{
	(void)t;
}
void SkyBox::scale(const float& s)
{
	(void)s;
}
void SkyBox::setGlobalTransform(const QMatrix4x4& g)
{
	(void)g;
}
void SkyBox::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
	cube->draw(shaderProgram, functions);
}
void SkyBox::initCube(float width, QImage& textureImg)
{
    float halfWidth = width / 2;
    QVector<VertexData> verteces;
    VertexData vertex11 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 1.0f, 2.0f/3.0f },QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex12 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 1.0f, 1.0f/3.0f },QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex13 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 3.0f/4.0f, 2.0f/3.0f }, QVector3D{ 0.0f, 0.0f, -1.0f } };
    VertexData vertex14 = { QVector3D{ halfWidth, -halfWidth, halfWidth },QVector2D{ 3.0f/4.0f, 1.0f/3.0f }, QVector3D{ 0.0f, 0.0f, -1.0f } };

    VertexData vertex21 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 3.0f/4.0f, 2.0f/3.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex22 = { QVector3D{ halfWidth, -halfWidth, halfWidth }, QVector2D{ 3.0f/4.0f, 1.0f/3.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex23 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 2.0f/3.0f },  QVector3D{ -1.0f, 0.0f, 0.0f } };
    VertexData vertex24 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 1.0f/3.0f }, QVector3D{ -1.0f, 0.0f, 0.0f } };

    VertexData vertex31 = { QVector3D{ halfWidth, halfWidth, halfWidth }, QVector2D{ 2.0f/4.0f, 1.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex32 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 2.0f/3.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex33 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 1.0f/4.0f, 1.0f },  QVector3D{ 0.0f, -1.0f, 0.0f } };
    VertexData vertex34 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 2.0f/3.0f }, QVector3D{ 0.0f, -1.0f, 0.0f } };

    VertexData vertex41 = { QVector3D{ halfWidth, halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 2.0f/3.0f }, QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex42 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 1.0f/3.0f }, QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex43 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 2.0f/3.0f },  QVector3D{ 0.0f, 0.0f, 1.0f } };
    VertexData vertex44 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 1.0f/3.0f }, QVector3D{ 0.0f, 0.0f, 1.0f } };

    VertexData vertex51 = { QVector3D{ -halfWidth, halfWidth, halfWidth }, QVector2D{ 0.0f, 2.0f/3.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex52 = { QVector3D{ -halfWidth, halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 2.0f/3.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex53 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 0.0f, 1.0f/3.0f },  QVector3D{ 1.0f, 0.0f, 0.0f } };
    VertexData vertex54 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 1.0f/3.0f }, QVector3D{ 1.0f, 0.0f, 0.0f } };

    VertexData vertex61 = { QVector3D{ -halfWidth, -halfWidth, halfWidth }, QVector2D{ 1.0f/4.0f, 0.0f }, QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex62 = { QVector3D{ -halfWidth, -halfWidth, -halfWidth }, QVector2D{ 1.0f/4.0f, 1.0f/3.0 }, QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex63 = { QVector3D{ halfWidth, -halfWidth, halfWidth }, QVector2D{ 2.0f/4.0f, 0.0f },  QVector3D{ 0.0f, 1.0f, 0.0f } };
    VertexData vertex64 = { QVector3D{ halfWidth, -halfWidth, -halfWidth }, QVector2D{ 2.0f/4.0f, 1.0f/3.0f }, QVector3D{ 0.0f, 1.0f, 0.0f } };


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
    QVector<IndexesMode> listIndexeModes;
    listIndexeModes.append(IndexesMode{ GL_TRIANGLES, QVector<GLuint>{} });
    for (int i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 3);
        indexes.append(i + 1);
        listIndexeModes[0].indexes.append(i + 0);
        listIndexeModes[0].indexes.append(i + 2);
        listIndexeModes[0].indexes.append(i + 1);
        listIndexeModes[0].indexes.append(i + 2);
        listIndexeModes[0].indexes.append(i + 3);
        listIndexeModes[0].indexes.append(i + 1);
    }

    Material* newMtl = new Material;
    newMtl->setDiffuseMap(textureImg);
    newMtl->setShinnes(96);
    newMtl->setDiffuseColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setAmbienceColor(QVector3D(1.0, 1.0, 1.0));
    newMtl->setSpecularColor(QVector3D(1.0, 1.0, 1.0));

    cube = new SimpleObject3D(verteces, indexes, newMtl, listIndexeModes);
}