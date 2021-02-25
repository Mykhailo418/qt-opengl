#ifndef OBJECT_ENGINE_3D_H
#define OBJECT_ENGINE_3D_H

#include "interfaces/transformational.h"

#include <QVector>
#include "SimpleObject3D.h"
#include "MaterialLibrary.h"

class ObjectEngine3D : public Transformational
{
public:
	ObjectEngine3D();
	void loadObjectFromFile(const QString &filename);
	void addObject(SimpleObject3D* object);
	SimpleObject3D* getObject(quint32 index);

	void rotate(const QQuaternion& r);
	void translate(const QVector3D& t);
	void scale(const float& s);
	void setGlobalTransform(const QMatrix4x4& g);
	void draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions);

private:
	QVector<SimpleObject3D*> objects;
	MaterialLibrary materialLibrary;

	GLenum getDrawMode(int vertecesCount);
};

#endif // OBJECT_ENGINE_3D_H