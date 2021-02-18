#ifndef GROUP_3D_H
#define GROUP_3D_H

#include "interfaces/transformational.h"

#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector>

class Group3D : public Transformational
{
public:
	Group3D();
	~Group3D();

	void rotate(const QQuaternion& r);
	void translate(const QVector3D& t);
	void scale(const float& s);
	void setGlobalTransform(const QMatrix4x4& g);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions);

	void addObject(Transformational* obj);
	void deleteObject(Transformational* obj);
	void deleteObject(const int &index);

private:
	QQuaternion rotation;
	QVector3D translation;
	float scaling = 1.0f;
	QMatrix4x4 globalTransformMatrix;

	QVector<Transformational*> objects;
};

#endif // GROUP_3D_H