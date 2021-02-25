#include "Material.h"
#include <QDebug>
#include <QFile>

Material::Material()
{

}
Material::~Material()
{

}
void Material::setName(const QString& mtlName) 
{
	_mtlName = mtlName;
}
const QString& Material::mtlName() const
{
	return _mtlName;
}
void Material::setDiffuseColor(const QVector3D& diffuseColor)
{
	_diffuseColor = diffuseColor;
}
const QVector3D& Material::diffuseColor() const
{
	return _diffuseColor;
}
void Material::setAmbienceColor(const QVector3D& ambienceColor)
{
	_ambienceColor = ambienceColor;
}
const QVector3D& Material::ambienceColor() const
{
	return _ambienceColor;
}
void Material::setSpecularColor(const QVector3D& specularColor)
{
	_specularColor = specularColor;
}
const QVector3D& Material::specularColor() const
{
	return _specularColor;
}
void Material::setShinnes(const qreal& shinnes)
{
	_shinnes = shinnes;
}
const float& Material::shinnes() const
{
	return _shinnes;
}
void Material::setDiffuseMap(const QString& filename)
{
	QImage image;
	image.load(filename);
	_diffuseMap = image;
	_isDiffMapUsed = true;
}
void Material::setDiffuseMap(const QImage& image)
{
	_diffuseMap = image;
	_isDiffMapUsed = true;
}
const QImage& Material::diffuseMap() const
{
	return _diffuseMap;
}
bool Material::isDiffMapUsed() const
{
	return _isDiffMapUsed;
}