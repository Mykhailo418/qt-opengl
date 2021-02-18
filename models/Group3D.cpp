#include "Group3D.h"

Group3D::Group3D()
{

}
Group3D::~Group3D()
{

}

void Group3D::translate(const QVector3D& t)
{
	translation += t;

	QMatrix4x4 localMatrix;
	localMatrix.setToIdentity();
	localMatrix.translate(translation);
	localMatrix.rotate(rotation);
	localMatrix.scale(scaling);
	localMatrix = globalTransformMatrix * localMatrix;

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setGlobalTransform(localMatrix);
	}
}

void Group3D::rotate(const QQuaternion& r)
{
	rotation = r * rotation;

	QMatrix4x4 localMatrix;
	localMatrix.setToIdentity();
	localMatrix.translate(translation);
	localMatrix.rotate(rotation);
	localMatrix.scale(scaling);
	localMatrix = globalTransformMatrix * localMatrix;

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setGlobalTransform(localMatrix);
	}
}
void Group3D::scale(const float& s)
{
	scaling *= s;

	QMatrix4x4 localMatrix;
	localMatrix.setToIdentity();
	localMatrix.translate(translation);
	localMatrix.rotate(rotation);
	localMatrix.scale(scaling);
	localMatrix = globalTransformMatrix * localMatrix;

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setGlobalTransform(localMatrix);
	}
}
void Group3D::setGlobalTransform(const QMatrix4x4& g)
{
	globalTransformMatrix = g;

	QMatrix4x4 localMatrix;
	localMatrix.setToIdentity();
	localMatrix.translate(translation);
	localMatrix.rotate(rotation);
	localMatrix.scale(scaling);
	localMatrix = globalTransformMatrix * localMatrix;

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->setGlobalTransform(localMatrix);
	}
}

void Group3D::addObject(Transformational* obj)
{
	objects.append(obj);
}

void Group3D::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw(shaderProgram, functions);
	}
}

void Group3D::deleteObject(Transformational* obj)
{
	objects.removeAll(obj);
}
void Group3D::deleteObject(const int& index)
{
	objects.remove(index);
}