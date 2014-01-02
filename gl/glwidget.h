#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QWheelEvent>

#include "glcamera.h"

#include "db/database.h"

#include "gl/glpoipoint.h"

#include "db/Relations/path.h"

#include "algorithms/pathalgorithms.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    GLCamera* camera;
    QTimer *timer;

    QPoint lastPos;

    int canvasWidth;
    int canvasHeight;

    bool isStop;
    bool isGLInitialized;

    map<unsigned long int,Way*>* allWays;
    vector<WaySegment*> path;
    Path* newPath;

    set<Path*,ComparePaths> radiusSearch;
    float searchRadius;

    QPointF *startPoint;
    QPointF *endPoint;

    vector<GLPOIPoint*> glPOIPoints;

    void drawPOIPoints();
    void scalePOIPoints(float);
    void drawPath();
    void drawRadiusSearch();

public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent*);

    void wheelEvent(QWheelEvent*);

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);

    void startGL();
    void stopGL();
    QPointF getGeoPosition(QPoint);
    void centerMap();

    void setMap(map<unsigned long int,Way*>*);

    void setPath(vector<WaySegment*>);
    void setPath(Path*);
    void deletePath();

    void setRadiusSearch(set<Path*,ComparePaths>, float);
    void deleteRadiusSearch();

    void setPOIs(vector<POIPoint*>);
    void deletePOIs();


    void drawStartPoint(QPointF*);
    void drawEndPoint(QPointF*);
    void deleteStartPoint();
    void deleteEndPoint();

signals:
    void poiClicked(POIPoint*, QMouseEvent*);

public slots:
    void updateLC();
};

#endif // GLWIDGET_H
