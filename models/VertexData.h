#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <QVector3D>
#include <QVector2D>

struct VertexData
{
    VertexData() {}
    VertexData(QVector3D p, QVector2D t, QVector3D n)
        : position(p), texCoord(t), normal(n) {}

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

#endif // VERTEX_DATA_H