#include "glcamera.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#endif

#include <QDebug>

GLCamera::GLCamera()
{
    moveSpeed = 10.0;

    x = 0.0;
    y = 0.0;
    z = 1.0;

    look_x = 0.0;
    look_y = 0.0;
    look_z = 0.0;

    zoomLevel = 1.0;
    zoomSpeed = 0.05;

    // lat="46.791803,9" lon="4.427704,7"
    planeLeft = 437.0;
    planeRight = 449.0;

    planeBottom = 4674.0;
    planeTop = 4686.0;

    planeNear = 0.0;
    planeFar = 10.0;

    canvasWidth = 0;
    canvasHeight = 0;
}

void GLCamera::left()
{
    x -= (moveSpeed * (1 / std::abs(zoomLevel)));
    look_x -= (moveSpeed * (1 / std::abs(zoomLevel)));
}

void GLCamera::right()
{
    x += (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_x += (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::up()
{
    y += (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_y += (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::down()
{
    y -= (moveSpeed * (1 / std::abs(zoomLevel)));;
    look_y -= (moveSpeed * (1 / std::abs(zoomLevel)));;
}

void GLCamera::move(int dx, int dy)
{
    // TODO : Need good scrolling
    x -= (dx * (0.10 * (1 / std::abs(zoomLevel))));
    look_x -= (dx * (0.10 * (1 / std::abs(zoomLevel))));
    y += (dy * (0.10 * (1 / std::abs(zoomLevel))));
    look_y += (dy * (0.10 * (1 / std::abs(zoomLevel))));
}

void GLCamera::zoomIn()
{
    if(zoomLevel == 121)
        return;

    if(planeRight - planeLeft <= 0.1 || planeTop - planeBottom <= 0.1)
        return;

    zoomLevel += 1;

    planeLeft += zoomSpeed;
    planeRight -= zoomSpeed;
    planeBottom += zoomSpeed;
    planeTop -= zoomSpeed;

    setViewingVolume();
}

void GLCamera::zoomOut()
{
    if(zoomLevel == 1)
        return;

    zoomLevel -= 1;

    planeLeft -= zoomSpeed;
    planeRight += zoomSpeed;
    planeBottom -= zoomSpeed;
    planeTop += zoomSpeed;

    setViewingVolume();
}

void GLCamera::setViewingVolume()
{
    glViewport(0, 0, canvasWidth, canvasHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(planeLeft,
            planeRight,
            planeBottom,
            planeTop,
            planeNear,
            planeFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLCamera::center()
{
    x = 0.0;
    y = 0.0;
    z = 1.0;

    look_x = 0.0;
    look_y = 0.0;
    look_z = 0.0;
}

