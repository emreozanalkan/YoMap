#include "glcamera.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#endif

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
    zoomSpeed = 0.1;

    // lat="46.791803,9" lon="4.427704,7"
    planeLeft = 438.0;
    planeRight = 448.0;

    planeBottom = 4675.0;
    planeTop = 4685.0;

    planeNear = 0.0;
    planeFar = 10.0;

    canvasWidth = 0;
    canvasHeight = 0;
}

void GLCamera::left()
{
    x -= moveSpeed;
    look_x -= moveSpeed;
}

void GLCamera::right()
{
    x += moveSpeed;
    look_x += moveSpeed;
}

void GLCamera::up()
{
    y += moveSpeed;
    look_y += moveSpeed;
}

void GLCamera::down()
{
    y -= moveSpeed;
    look_y -= moveSpeed;
}

void GLCamera::move(int dx, int dy)
{
    // TODO : Need good scrolling
    x -= (dx * (0.15 * (1 / zoomLevel)));
    look_x -= (dx * (0.15 * (1 / zoomLevel)));
    y += (dy * (0.15 * (1 / zoomLevel)));
    look_y += (dy * (0.15 * (1 / zoomLevel)));
}

void GLCamera::zoomIn()
{
//    if(zoomLevel > 50)
//        return;

    zoomLevel += 1;

    if(planeRight - planeLeft <= 0.2 || planeTop - planeBottom <= 0.2)
        return;

    planeLeft += zoomSpeed;
    planeRight -= zoomSpeed;
    planeBottom += zoomSpeed;
    planeTop -= zoomSpeed;

    setViewingVolume();
}

void GLCamera::zoomOut()
{
//    if(zoomLevel < -8)
//        return;

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

