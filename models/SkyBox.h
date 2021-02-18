#ifndef SKYBOX_H
#define SKYBOX_H

#include "../interfaces/transformational.h" 

#include <QImage>
#include <QQuaternion>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class SimpleObject3D;

class SkyBox : Transformational
{
public:
	SkyBox(float width, QImage &textureImg);
	~SkyBox();

	void rotate(const QQuaternion& r);
	void translate(const QVector3D& t);
	void scale(const float& s);
	void setGlobalTransform(const QMatrix4x4& g);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions = nullptr);

private:
	void initCube(float width, QImage& textureImg);

	SimpleObject3D* cube;
};

#endif // SKYBOX_H