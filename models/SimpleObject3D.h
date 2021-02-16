#ifndef SIMPLE_OBJECT_3D_H
#define SIMPLE_OBJECT_3D_H

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

class SimpleObject3D
{
public:
	SimpleObject3D();
	SimpleObject3D(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, const QImage &textureImg);
	~SimpleObject3D();

	void init(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, const QImage &textureImg);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions);
	void translate(const QVector3D& translateVector);

private:
	QOpenGLBuffer vertexBuffer;
	QOpenGLBuffer indexBuffer;
	QMatrix4x4 matrixProjection;
	QOpenGLTexture* texture;
};

#endif // SIMPLE_OBJECT_3D_H