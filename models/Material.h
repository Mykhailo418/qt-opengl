#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QVector3D>
#include <QImage>

class Material
{
public:
	Material();
	~Material();
	void setName(const QString& mtlName);
	const QString& mtlName() const;
	void setDiffuseColor(const QVector3D& diffuseColor);
	const QVector3D& diffuseColor() const;
	void setAmbienceColor(const QVector3D& ambienceColor);
	const QVector3D& ambienceColor() const;
	void setSpecularColor(const QVector3D& specularColor);
	const QVector3D& specularColor() const;
	void setShinnes(const qreal& shinnes);
	const float& shinnes() const;
	void setDiffuseMap(const QString& filename);
	void setDiffuseMap(const QImage& image);
	const QImage& diffuseMap() const;
	bool isDiffMapUsed() const;
	void setNormalMap(const QString& filename);
	void setNormalMap(const QImage& image);
	const QImage& normalMap() const;
	bool isNormalMapUsed() const;

private:
	QString _mtlName;
	QVector3D _diffuseColor;
	QVector3D _ambienceColor;
	QVector3D _specularColor;
	float _shinnes;
	QImage _diffuseMap;
	QImage _normalMap;
	bool _isDiffMapUsed = false;
	bool _isNormalMapUsed = false;
};

#endif // MATERIAL_H