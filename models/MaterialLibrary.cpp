#include <iostream>
#include "MaterialLibrary.h"
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include "Material.h"

MaterialLibrary::MaterialLibrary()
{

}
MaterialLibrary::~MaterialLibrary()
{

}
void MaterialLibrary::addMaterial(Material* material)
{
	if (!material) {
		return;
	}
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i] == material) {
			return;
		}
	}
	materials.append(material);
}
Material* MaterialLibrary::getMaterial(quint32 index)
{
	if (materials.size() > index) {
		return materials[index];
	}
	return 0;
}
Material* MaterialLibrary::getMaterial(const QString& mtlName)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i]->mtlName() == mtlName) {
			return materials[i];
		}
	}
	return 0;
}
quint32 MaterialLibrary::getCountMaterials()
{
	return materials.size();
}
void MaterialLibrary::loadMaterialsFromFile(const QString& mtlFilename)
{
	QFile mtlFile(mtlFilename);
	if (!mtlFile.exists()) {
		qDebug() << "Can not open mtl file" << mtlFilename;
		return;
	}
	mtlFile.open(QIODevice::ReadOnly);
	for (int i = 0; i < materials.size(); i++) {
		delete materials[i];
	}
	materials.clear();
	QFileInfo fileInfo(mtlFilename);
	QTextStream inputStream(&mtlFile);
	Material* newMtl = 0;

	while (!inputStream.atEnd()) {
		QString str = inputStream.readLine();
		str.replace("\t", "");
		str.replace("\n", "");
		QStringList list = str.split(" ");
		list.removeAll("");
		if (list.size() < 1) {
			continue;
		}
		if (list[0] == "#") {
			// comment
		}
		else if (list[0] == "newmtl") {
			addMaterial(newMtl);
			newMtl = new Material;
			newMtl->setName(list[1]);
		}
		else if (list[0] == "Ns") {
			newMtl->setShinnes(list[1].toFloat());
		}
		else if (list[0] == "Ka") {
			newMtl->setAmbienceColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
		}
		else if (list[0] == "Kd") {
			newMtl->setDiffuseColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
		}
		else if (list[0] == "Ks") {
			newMtl->setSpecularColor(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
		}
		else if (list[0] == "map_Kd") {
			QString fileName;
			for (int i = 1; i < list.size(); i++) {
				if (fileName.isEmpty()) {
					fileName = list[i];
				}
				else {
					fileName += " " + list[i];
				}
			}
			if (!fileName.contains("/") && !fileName.contains("\\")) {
				fileName = QString("%1/%2")
					.arg(fileInfo.absolutePath())
					.arg(fileName);
			}
			//qDebug() << fileName;
			newMtl->setDiffuseMap(fileName);
		}
		else if (list[0] == "bump" || list[0] == "map_Bump") {
			QString fileName = list[1];
			if (!fileName.contains("/") && !fileName.contains("\\")) {
				fileName = QString("%1/%2")
					.arg(fileInfo.absolutePath())
					.arg(fileName);
			}
			//qDebug() << fileName;
			newMtl->setNormalMap(fileName);
		}
	}
	addMaterial(newMtl);
	qDebug() << "Material Library file close";
	mtlFile.close();
}