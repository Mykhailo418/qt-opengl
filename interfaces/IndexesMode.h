#include <QVector>
#pragma once

#ifndef INDEXES_MODE_H
#define INDEXES_MODE_H

struct IndexesMode {
	GLenum mode;
	QVector<GLuint> indexes;
};

#endif // INDEXES_MODE_H