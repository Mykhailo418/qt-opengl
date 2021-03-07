#include "SimpleObject3D.h"
#include <iostream>
#include "Material.h"

SimpleObject3D::SimpleObject3D() : indexBuffer(QOpenGLBuffer::IndexBuffer), diffuseMap(0)
{

}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, Material* materialObj, const QVector<IndexesMode> &modes)
	: indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	init(vertData, indexes, materialObj, modes);
}

SimpleObject3D::~SimpleObject3D()
{
	if (vertexBuffer.isCreated()) {
		vertexBuffer.destroy();
	}
	if (indexBuffer.isCreated()) {
		indexBuffer.destroy();
	}
	if (diffuseMap != 0 && diffuseMap->isCreated()) {
		diffuseMap->destroy();
	}
}

void SimpleObject3D::init(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, Material* materialObj, const QVector<IndexesMode> &modes)
{
	listIndexeModes = modes;
	if (vertexBuffer.isCreated()) {
		vertexBuffer.destroy();
	}
	if (indexBuffer.isCreated()) {
		indexBuffer.destroy();
	}
	/*if (texture && texture != nullptr && texture != 0 && texture != NULL) {
		if (texture->isCreated()) {
			delete texture;
			texture = 0;
		}
	}*/

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.allocate(vertData.constData(), vertData.size() * sizeof(VertexData));
	vertexBuffer.release();

	indexBuffer.create();
	indexBuffer.bind();
	indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
	indexBuffer.release();

	material = materialObj ? materialObj : new Material();
	QImage textureImg;
	if (material->isDiffMapUsed()) {
		qDebug() << material->mtlName();
		textureImg = material->diffuseMap().mirrored();
	}
	else {
		textureImg = QImage(1, 1, QImage::Format_RGB32);
		QRgb value;
		value = qRgb(200, 200, 200);
		textureImg.setPixel(0, 0, value);
	}

	diffuseMap = new QOpenGLTexture(textureImg);
	diffuseMap->setMinificationFilter(QOpenGLTexture::Nearest);
	diffuseMap->setMinificationFilter(QOpenGLTexture::Linear);
	diffuseMap->setWrapMode(QOpenGLTexture::Repeat);

	if (material->isNormalMapUsed()) {
		QImage normalMapImg = material->normalMap().mirrored();
		normalMap = new QOpenGLTexture(normalMapImg);
		normalMap->setMinificationFilter(QOpenGLTexture::Nearest);
		normalMap->setMinificationFilter(QOpenGLTexture::Linear);
		normalMap->setWrapMode(QOpenGLTexture::Repeat);
	}
}

void SimpleObject3D::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
	if (!vertexBuffer.isCreated() || !indexBuffer.isCreated()) {
		return;
	}
	diffuseMap->bind(0);
	shaderProgram->setUniformValue("u_diffuseMap", 0);

	if (material->isNormalMapUsed()) {
		normalMap->bind(1);
		shaderProgram->setUniformValue("u_normalMap", 1);
	}

	QMatrix4x4 matrixProjection;
	matrixProjection.setToIdentity();
	matrixProjection.translate(translation);
	matrixProjection.rotate(rotation);
	matrixProjection.scale(scaling);
	matrixProjection = globalTransformMatrix * matrixProjection;

	shaderProgram->setUniformValue("u_modelMatrix", matrixProjection);
	shaderProgram->setUniformValue("u_materialProperty.diffuseColor", material->diffuseColor());
	shaderProgram->setUniformValue("u_materialProperty.ambienceColor", material->ambienceColor());
	shaderProgram->setUniformValue("u_materialProperty.specularColor", material->specularColor());
	shaderProgram->setUniformValue("u_materialProperty.shinnes", material->shinnes());
	shaderProgram->setUniformValue("u_isUsingDiffuseMap", material->isDiffMapUsed());
	shaderProgram->setUniformValue("u_isUsingNormalMap", material->isNormalMapUsed());

	vertexBuffer.bind();

	int offset = 0;
	int vertLoc = shaderProgram->attributeLocation("a_position");
	shaderProgram->enableAttributeArray(vertLoc);
	shaderProgram->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(QVector3D);

	int texLoc = shaderProgram->attributeLocation("a_texCoord");
	shaderProgram->enableAttributeArray(texLoc);
	shaderProgram->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

	offset += sizeof(QVector3D);

	int normalLoc = shaderProgram->attributeLocation("a_normal");
	shaderProgram->enableAttributeArray(normalLoc);
	shaderProgram->setAttributeBuffer(normalLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(QVector3D);

	int tangentLoc = shaderProgram->attributeLocation("a_tangent");
	shaderProgram->enableAttributeArray(tangentLoc);
	shaderProgram->setAttributeBuffer(tangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(QVector3D);

	int bitangentLoc = shaderProgram->attributeLocation("a_bitangent");
	shaderProgram->enableAttributeArray(bitangentLoc);
	shaderProgram->setAttributeBuffer(bitangentLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

	indexBuffer.bind();
	if (listIndexeModes.size() > 1) {
		for (IndexesMode indMode : listIndexeModes) {
			functions->glDrawArrays(indMode.mode, indMode.indexes[0], indMode.indexes.size());
		}
	}
	else {
		//functions->glDrawArrays(listIndexeModes[0].mode, listIndexeModes[0].indexes[0], listIndexeModes[0].indexes.size());
		functions->glDrawElements(listIndexeModes[0].mode, indexBuffer.size(), GL_UNSIGNED_INT, 0);
	}

	/*for (IndexesMode indMode : listIndexeModes) {
		functions->glDrawArrays(indMode.mode, indMode.indexes[0], indMode.indexes.size());
	}*/
	//functions->glDrawArrays(GL_TRIANGLES, 0, indexBuffer.size());
	//functions->glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

	vertexBuffer.release();
	indexBuffer.release();
	diffuseMap->release();
	if (material->isNormalMapUsed()) {
		normalMap->release();
	}
}

void SimpleObject3D::translate(const QVector3D& t)
{
	translation += t;
}

void SimpleObject3D::rotate(const QQuaternion& r)
{
	rotation = r * rotation;
}
void SimpleObject3D::scale(const float& s)
{
	scaling *= s;
}
void SimpleObject3D::setGlobalTransform(const QMatrix4x4& g)
{
	globalTransformMatrix = g;
}