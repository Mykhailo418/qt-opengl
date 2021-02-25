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

private:
	QString _mtlName;
	QVector3D _diffuseColor;
	QVector3D _ambienceColor;
	QVector3D _specularColor;
	float _shinnes;
	QImage _diffuseMap;
	bool _isDiffMapUsed = false;
};

#endif // MATERIAL_H