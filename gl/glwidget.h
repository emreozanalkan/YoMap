#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>

#include "glcamera.h"

#include "db/database.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    GLCamera* camera;
    QTimer *timer;

    QPoint lastPos;

    int canvasWidth;
    int canvasHeight;

    QPoint lastRightClickPos;

    Database* db;

    bool isStop;

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent*);

    void wheelEvent(QWheelEvent*);

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    //void mouseReleaseEvent(QMouseEvent *);

    void startGL();
    void stopGL();
    QPointF getGeoPosition(QPoint);
    void centerMap();
    //void dataPassing

signals:


public slots:
    void updateLC();
//    void setStartPoint();
//    void setEndPoint();
};

#endif // GLWIDGET_H
