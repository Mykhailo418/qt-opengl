#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include "interfaces/transformational.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Camera3D : Transformational
{
public:
	Camera3D();
	~Camera3D();

	void rotate(const QQuaternion& r);
	void translate(const QVector3D& t);
	void scale(const float& s);
	void setGlobalTransform(const QMatrix4x4& g);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions = nullptr);

private:
	QQuaternion rotation;
	QVector3D translation;
	float scaling = 1.0f;
	QMatrix4x4 globalTransformMatrix;
	QMatrix4x4 viewMatrix;
};

#endif // CAMERA_3D_H