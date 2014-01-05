#ifndef GLPOIPOINT_H
#define GLPOIPOINT_H

#include "db/poi/poipoint.h"

//#ifdef __APPLE__
//    #include <GLUT/glut.h>
//#else
//    #include <glut.h>
//#endif

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class GLPOIPoint
{
public:
    GLPOIPoint();

    POIPoint* point;

    double textureSize;

    unsigned int texture;

    QVector<QVector3D> vertices;

    QVector<QVector2D> textureCoordinates;

    void setupVertices();

    void setupVertices(double scale);

    bool isContains(QPointF);
};

#endif // GLPOIPOINT_H
