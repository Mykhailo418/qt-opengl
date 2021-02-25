#include "ObjectEngine3D.h"
#include <iostream>
#include <QFIle>
#include <QFIleInfo>
#include <QQuaternion>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

ObjectEngine3D::ObjectEngine3D()
{

}
void ObjectEngine3D::loadObjectFromFile(const QString& filename)
{
    QFile objFile(filename);
    if (!objFile.exists()) {
        qDebug() << "File not found";
        return;
    }
    objFile.open(QIODevice::ReadOnly);
    QTextStream input(&objFile);
    QVector<QVector3D> coords;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;

    QVector<VertexData> verteces;
    QVector<GLuint> indexes;
    QVector<IndexesMode> listIndexeModes;
    SimpleObject3D* object = 0;
    QString mtlName;

    while (!input.atEnd()) {
        QString str = input.readLine();
        qDebug() << str;
        QStringList list = str.split(" ");
        list.removeAll("");
       /* QStringList list;
        for (QString str : rawList) {
            if (str != "") {
               
                list.append(str);
            }
        }*/
        if (list.size() < 1) {
            continue;
        }

        if (list[0] == "#") {
            // comment 
            continue;
        }
        else if (list[0] == "mtllib") {
            // file with meterials
            QFileInfo info(filename);
            materialLibrary.loadMaterialsFromFile(
                QString("%1/%2")
                    .arg(info.absolutePath())
                    .arg(list[1])
            );
            continue;
        }
        else if (list[0] == "v") {
            // verteces coordinates
            coords.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        }
        else if (list[0] == "vt") {
            // textures coordinates
            texCoords.append(QVector2D(list[1].toFloat(), list[2].toFloat()));
            continue;
        }
        else if (list[0] == "vn") {
            // normals coordinates
            normals.append(QVector3D(list[1].toFloat(), list[2].toFloat(), list[3].toFloat()));
            continue;
        }
        else if (list[0] == "f") {
            // polygons
            int polygonSize = list.size();
            IndexesMode currentIndMode;
            GLenum currentMode = getDrawMode(polygonSize - 1);
            if (listIndexeModes.size() > 0) {
                currentIndMode = listIndexeModes[listIndexeModes.size() - 1];
            }
            if (listIndexeModes.size() == 0 || currentIndMode.mode != currentMode) {
                currentIndMode = IndexesMode{ currentMode, QVector<GLuint>{} };
                listIndexeModes.append(currentIndMode);
            }

            for (int i = 1; i < polygonSize; i++) {
                QStringList values = list[i].split("/");
                long vertIndex = values.size() > 0 ? values[0].toLong() : 0;
                long texIndex = values.size() > 1 ? values[1].toLong() : 0;
                long normalIndex = values.size() > 2 ? values[2].toLong() : 0;
                verteces.append(VertexData(
                    vertIndex != 0 ? coords[vertIndex - 1] : QVector3D(),
                    texIndex != 0 ? texCoords[texIndex - 1] : QVector2D(),
                    normalIndex != 0 ? normals[normalIndex - 1] : QVector3D()
                ));
                listIndexeModes[listIndexeModes.size() - 1].indexes.append(indexes.size());
                indexes.append(indexes.size());
            }
            continue;
        }
        else if (list[0] == "usemtl") {
            if (object) {
                object->init(verteces, indexes, materialLibrary.getMaterial(mtlName), listIndexeModes);
            }
            mtlName = list[1];
            addObject(object);
            object = new SimpleObject3D;
            verteces.clear();
            indexes.clear();
            listIndexeModes.clear();
        }
    }
    if (object) {
        object->init(verteces, indexes, materialLibrary.getMaterial(mtlName), listIndexeModes);
    }
    addObject(object);
    qDebug() << "Object Engine file close";
    objFile.close();
}
void ObjectEngine3D::addObject(SimpleObject3D* object)
{
    if (!object) {
        return;
    }
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == object) {
            return;
        }
    }
    objects.append(object);
}
SimpleObject3D* ObjectEngine3D::getObject(quint32 index)
{
    if (index < (quint32)objects.size()) {
        return objects[index];
    }
    else {
        return 0;
    }
}

GLenum ObjectEngine3D::getDrawMode(int vertecesCount)
{
    if (vertecesCount == 3) {
        return GL_TRIANGLES;
    }
    else if (vertecesCount == 4) {
        return GL_QUADS;
    }
    return GL_POLYGON;
}

void ObjectEngine3D::rotate(const QQuaternion& r)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->rotate(r);
    }
}
void ObjectEngine3D::translate(const QVector3D& t)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->translate(t);
    }
}
void ObjectEngine3D::scale(const float& s)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->scale(s);
    }
}
void ObjectEngine3D::setGlobalTransform(const QMatrix4x4& g)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->setGlobalTransform(g);
    }
}
void ObjectEngine3D::draw(QOpenGLShaderProgram* shaderProgram, QOpenGLFunctions* functions)
{
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw(shaderProgram, functions);
    }
}