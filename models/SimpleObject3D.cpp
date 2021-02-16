#include "SimpleObject3D.h"

SimpleObject3D::SimpleObject3D() : indexBuffer(QOpenGLBuffer::IndexBuffer), texture(0)
{

}

SimpleObject3D::SimpleObject3D(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, const QImage &textureImg)
	: indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	init(vertData, indexes, textureImg);
}

SimpleObject3D::~SimpleObject3D()
{
	if (vertexBuffer.isCreated()) {
		vertexBuffer.destroy();
	}
	if (indexBuffer.isCreated()) {
		indexBuffer.destroy();
	}
	if (texture != 0 && texture->isCreated()) {
		texture->destroy();
	}
}

void SimpleObject3D::init(const QVector<VertexData> &vertData, const QVector <GLuint> &indexes, const QImage &textureImg)
{
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

	texture = new QOpenGLTexture(textureImg.mirrored());
	texture->setMinificationFilter(QOpenGLTexture::Nearest);
	texture->setMinificationFilter(QOpenGLTexture::Linear);
	texture->setWrapMode(QOpenGLTexture::Repeat);

	matrixProjection.setToIdentity();
}

void SimpleObject3D::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
	if (!vertexBuffer.isCreated() || !indexBuffer.isCreated()) {
		return;
	}

	texture->bind(0);
	shaderProgram->setUniformValue("u_modelMatrix", matrixProjection);
	shaderProgram->setUniformValue("u_texture", 0);

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

	indexBuffer.bind();

	functions->glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);

	vertexBuffer.release();
	indexBuffer.release();
	texture->release();
}

void SimpleObject3D::translate(const QVector3D& translateVector)
{
	matrixProjection.translate(translateVector);
}