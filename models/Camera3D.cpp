#include "Camera3D.h"

Camera3D::Camera3D()
{
	globalTransformMatrix.setToIdentity();
}
Camera3D::~Camera3D()
{

}

void Camera3D::translate(const QVector3D& t)
{
	translation += t;

	viewMatrix.setToIdentity();
	viewMatrix.translate(translation);
	viewMatrix.rotate(rotation);
	viewMatrix.scale(scaling);
	viewMatrix = viewMatrix * globalTransformMatrix.inverted();
}

void Camera3D::rotate(const QQuaternion& r)
{
	rotation = r * rotation; // need * rotation in order to rotation calculate from previous rotation

	viewMatrix.setToIdentity();
	viewMatrix.translate(translation);
	viewMatrix.rotate(rotation);
	viewMatrix.scale(scaling);
	viewMatrix = viewMatrix * globalTransformMatrix.inverted();
}
void Camera3D::scale(const float& s)
{
	scaling *= s;

	viewMatrix.setToIdentity();
	viewMatrix.translate(translation);
	viewMatrix.rotate(rotation);
	viewMatrix.scale(scaling);
	viewMatrix = viewMatrix * globalTransformMatrix.inverted();
}
void Camera3D::setGlobalTransform(const QMatrix4x4& g)
{
	globalTransformMatrix = g;

	viewMatrix.setToIdentity();
	viewMatrix.translate(translation);
	viewMatrix.rotate(rotation);
	viewMatrix.scale(scaling);
	viewMatrix = viewMatrix * globalTransformMatrix.inverted();
}

void Camera3D::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
	if (functions != nullptr) {
		return;
	}

	shaderProgram->setUniformValue("u_viewMatrix", viewMatrix);
}