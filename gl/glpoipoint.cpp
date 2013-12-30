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
    vertices.append(QVector3D((geoPos.x() * 100.0f) + 0.1f, (geoPos.y() * 100.0f) + 0.1f, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) - 0.1f, (geoPos.y() * 100.0f) + 0.1f, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) - 0.1f, (geoPos.y() * 100.0f) - 0.1f, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) + 0.1f, (geoPos.y() * 100.0f) - 0.1f, 0.3f));
}

void GLPOIPoint::setupVertices(float scale)
{
    vertices.clear();
    boost_xy_point geoPos = point->getGeoPosition();

    if(scale > 1.0f)
        scale = 1.0f;

    vertices.append(QVector3D((geoPos.x() * 100.0f) + 0.1f * scale, (geoPos.y() * 100.0f) + 0.1f * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) - 0.1f * scale, (geoPos.y() * 100.0f) + 0.1f * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) - 0.1f * scale, (geoPos.y() * 100.0f) - 0.1f * scale, 0.3f));
    vertices.append(QVector3D((geoPos.x() * 100.0f) + 0.1f * scale, (geoPos.y() * 100.0f) - 0.1f * scale, 0.3f));
}
