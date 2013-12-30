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
}

void GLPOIPoint::setupVertices()
{
    vertices.clear();
    boost_xy_point geoPos = point->getGeoPosition();
    vertices.append(QVector3D((geoPos.x() * 100.0) + 0.1, (geoPos.y() * 100.0) + 0.1, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - 0.1, (geoPos.y() * 100.0) + 0.1, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - 0.1, (geoPos.y() * 100.0) - 0.1, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) + 0.1, (geoPos.y() * 100.0) - 0.1, 0.3f));
}

void GLPOIPoint::setupVertices(double scale)
{
    vertices.clear();
    boost_xy_point geoPos = point->getGeoPosition();

    if(scale > 1.0)
        scale = 1.0;

    vertices.append(QVector3D((geoPos.x() * 100.0) + 0.1 * scale, (geoPos.y() * 100.0) + 0.1 * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - 0.1 * scale, (geoPos.y() * 100.0) + 0.1 * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) - 0.1 * scale, (geoPos.y() * 100.0) - 0.1 * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0) + 0.1 * scale, (geoPos.y() * 100.0) - 0.1 * scale, 0.3f));
}
