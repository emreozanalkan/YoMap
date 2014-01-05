#include "glpoipoint.h"

#include <QString>
#include <QPixmap>
#include <QDebug>

GLPOIPoint::GLPOIPoint()
{
    textureCoordinates.append(QVector2D(1, 1));
    textureCoordinates.append(QVector2D(0, 1));
    textureCoordinates.append(QVector2D(0, 0));
    textureCoordinates.append(QVector2D(1, 0));

    texture = 0;

    textureSize = 0.20;
}

void GLPOIPoint::setupVertices()
{
    vertices.clear();
    boost_xy_point geoPos = point->getGeoPosition();
    vertices.append(QVector3D((geoPos.x() * 100.0) + textureSize, (geoPos.y() * 100.0) + textureSize, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - textureSize, (geoPos.y() * 100.0) + textureSize, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - textureSize, (geoPos.y() * 100.0) - textureSize, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) + textureSize, (geoPos.y() * 100.0) - textureSize, 0.5f));
}

void GLPOIPoint::setupVertices(double scale)
{
    vertices.clear();
    boost_xy_point geoPos = point->getGeoPosition();

    if(scale > 1.0)
        scale = 1.0;

    vertices.append(QVector3D((geoPos.x() * 100.0) + textureSize * scale, (geoPos.y() * 100.0) + textureSize * scale, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - textureSize * scale, (geoPos.y() * 100.0) + textureSize * scale, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - textureSize * scale, (geoPos.y() * 100.0) - textureSize * scale, 0.5f));
    vertices.append(QVector3D((geoPos.x() * 100.0) + textureSize * scale, (geoPos.y() * 100.0) - textureSize * scale, 0.5f));
}

bool GLPOIPoint::isContains(QPointF point)
{
    if(point.x() * 100.0 > vertices[0].x() || point.y() * 100.0 > vertices[0].y())
        return false;
    else if(point.x() * 100.0 < vertices[1].x() || point.y() * 100.0 > vertices[1].y())
        return false;
    else if(point.x() * 100.0 < vertices[2].x() || point.y() * 100.0 < vertices[2].y())
        return false;
    else if(point.x() * 100.0 > vertices[3].x() || point.y() * 100.0 < vertices[3].y())
        return false;
    else
        return true;
}
