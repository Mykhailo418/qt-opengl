#ifndef SIMPLE_OBJECT_3D_H
#define SIMPLE_OBJECT_3D_H

#include "interfaces/transformational.h"

#include <QVector>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <Qvector2D>
#include <QVector3D>

#include "VertexData.h"
#include "interfaces/IndexesMode.h"

class Material;

class SimpleObject3D : public Transformational
{
public:
	SimpleObject3D();
	SimpleObject3D(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, Material* materialObj, const QVector<IndexesMode> &modes);
	~SimpleObject3D();

	void init(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, Material* materialObj, const QVector<IndexesMode> &modes);
	void rotate(const QQuaternion& r);
	void translate(const QVector3D& t);
	void scale(const float& s);
	void setGlobalTransform(const QMatrix4x4& g);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions);

private:
	QOpenGLBuffer vertexBuffer;
	QOpenGLBuffer indexBuffer;
	QOpenGLTexture* diffuseMap;
	QOpenGLTexture* normalMap;

	QQuaternion rotation;
	QVector3D translation;
	float scaling = 1.0f;
	QMatrix4x4 globalTransformMatrix;
	QVector<IndexesMode> listIndexeModes;
	Material* material;
};

#endif // SIMPLE_OBJECT_3D_H