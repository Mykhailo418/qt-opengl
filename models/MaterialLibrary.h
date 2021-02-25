#ifndef MATERIAL_LIB_H
#define MATERIAL_LIB_H

#include <QVector>

class Material;

class MaterialLibrary
{
public:
	MaterialLibrary();
	~MaterialLibrary();
	void addMaterial(Material* material);
	Material* getMaterial(quint32 index);
	Material* getMaterial(const QString& mtlName);
	quint32 getCountMaterials();
	void loadMaterialsFromFile(const QString &mtlFilename);

private:
	QVector<Material*> materials;
};

#endif // MATERIAL_LIB_H