#ifndef GLCAMERA_H
#define GLCAMERA_H

class GLCamera
{
public:
    GLCamera();

    float x;
    float y;
    float z;

    float look_x;
    float look_y;
    float look_z;

    float moveSpeed;

    float zoomLevel;

    float zoomSpeed;


    double planeLeft;
    double planeRight;
    double planeBottom;
    double planeTop;
    double planeNear;
    double planeFar;

    int canvasWidth;
    int canvasHeight;


    void left();
    void right();
    void up();
    void down();

    void move(int, int);

    void zoomIn();
    void zoomOut();

    void setViewingVolume();
};

#endif // GLCAMERA_H
