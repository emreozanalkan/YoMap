#include "glwidget.h"

#include <QtOpenGL>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <glut.h>
#endif

#include <QDebug>

#include "db/database.h"
#include "algorithms/pathalgorithms.h"
#include "../stable.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    canvasWidth = 0;
    canvasHeight = 0;
    timer = new QTimer(this);
    camera = new GLCamera();

    isStop = true;
    isGLInitialized = false;

    startPoint = 0;
    endPoint = 0;

}

void GLWidget::initializeGL()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glClearColor(0.329412, 0.329412, 0.329412, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);

    //glEnable( GL_POINT_SPRITE );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateLC()));
    this->setFocus();

    if(!glPOIPoints.empty())
    {
        for(int i = 0; i < glPOIPoints.size(); i++)
        {
            glPOIPoints[i]->texture = bindTexture(QPixmap(QString::fromStdString(glPOIPoints[i]->point->getIconPath())), GL_TEXTURE_2D);
        }
    }

    isGLInitialized = true;
}

void GLWidget::updateLC()
{
    // Do my update stuff

    //map->update();

    updateGL();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camera->x, camera->y, camera->z,
              camera->look_x, camera->look_y, camera->look_z,
              0.0, 1.0, 0.0);

    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    glLineWidth(1.0f);

    if(allWays == NULL)
        return;
    map<unsigned long int, Way*>::iterator wayIt;
    for(wayIt = allWays->begin(); wayIt != allWays->end(); ++wayIt)
    {
        Way* way = ((Way*)(*wayIt).second);
        switch(way->getWayType())
        {
            case ns_way::primary:
            default:
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            case ns_way::footway:
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
            case ns_way::steps:
                glColor3f(0.0f, 1.0f, 1.0f);
                break;
            case ns_way::track:
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            case ns_way::raceway:
                glColor3f(1.0f, 0.5f, 0.0f);
                break;
            case ns_way::service:
                glColor3f(0.737255f, 0.560784f, 0.560784f);
                break;
        }

        glBegin(GL_LINE_STRIP);
        for (vector<Node*>::iterator nodeIt = way->getNodesBegin(); nodeIt != way->getNodesEnd(); nodeIt++){
            boost_xy_point& nodeGeoPos = (*nodeIt)->getGeoPosition();
            glVertex3f(nodeGeoPos.x() * 100, nodeGeoPos.y() * 100, 0.0f);
        }
        glEnd();
    }

    glLineWidth(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        if(startPoint != NULL && !path.empty())
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(startPoint->x() * 100.0f, startPoint->y() * 100.0f, 0.1f);
            WaySegment* waySegment = path[0];
            boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
            glVertex3f(nodeGeoPosA.x() * 100.0f, nodeGeoPosA.y() * 100.0f, 0.1f);
        }
        glColor3f(1.0f, 0.0f, 0.0f);
        int i;
        for(i = 0; i < path.size(); i++)
        {
            WaySegment* waySegment = path[i];
            boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
            boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
            //glBegin(GL_LINES);
            //qDebug() << "A: " <<nodeGeoPosA.x() << "-" << nodeGeoPosA.y();
            //qDebug() << "B: " <<nodeGeoPosB.x() << "-" << nodeGeoPosB.y();
            glVertex3f(nodeGeoPosA.x() * 100.0f, nodeGeoPosA.y() * 100.0f, 0.1f);
            glVertex3f(nodeGeoPosB.x() * 100.0f, nodeGeoPosB.y() * 100.0f, 0.1f);
            //glEnd();
        }
        if(endPoint != NULL && !path.empty())
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            WaySegment* waySegment = path[i-1];
            boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
            glVertex3f(nodeGeoPosB.x() * 100.0f, nodeGeoPosB.y() * 100.0f, 0.1f);
            glVertex3f(endPoint->x() * 100.0f, endPoint->y() * 100.0f, 0.1f);
        }
        glEnd();


    if(startPoint != NULL)
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glPointSize(7.0f);
        glBegin(GL_POINTS);
        glVertex3f(startPoint->x() * 100.0f, startPoint->y() * 100.0f, 0.2f);
        glEnd();
    }

    if(endPoint != NULL)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(7.0f);
        glBegin(GL_POINTS);
        glVertex3f(endPoint->x() * 100.0f, endPoint->y() * 100.0f, 0.2f);
        glEnd();
    }


    glPushMatrix();
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    //glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(0.3f, 0.3f, 0.3f);
    glEnable(GL_TEXTURE_2D);
    for(int i = 0; i < glPOIPoints.size(); i++)
    {
        if(!glPOIPoints[i]->texture)
            continue;
        glVertexPointer(3, GL_FLOAT, 0, glPOIPoints[i]->vertices.constData());
        glTexCoordPointer(2, GL_FLOAT, 0, glPOIPoints[i]->textureCoordinates.constData());
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, glPOIPoints[i]->texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    camera->canvasWidth = w;
    camera->canvasHeight = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = float(w) / float(h);

    glOrtho(camera->planeLeft,
            camera->planeRight,
            camera->planeBottom,
            camera->planeTop,
            camera->planeNear,
            camera->planeFar);

    //gluPerspective(60.0, ratio, 1.0, 100.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_C:
        camera->center();
        break;
    case Qt::Key_W:
        camera->up();
        break;
    case Qt::Key_A:
        camera->left();
        break;
    case Qt::Key_S:
        camera->down();
        break;
    case Qt::Key_D:
        camera->right();
        break;
    case Qt::Key_Q:
        camera->zoomIn();
        break;
    case Qt::Key_E:
        camera->zoomOut();
        break;
    case Qt::Key_Up:
        camera->up();
        break;
    case Qt::Key_Down:
        camera->down();
        break;
    case Qt::Key_Left:
        camera->left();
        break;
    case Qt::Key_Right:
        camera->right();
        break;
    case Qt::Key_Plus:
        camera->zoomIn();
        break;
    case Qt::Key_Minus:
        camera->zoomOut();
        break;
    case Qt::Key_Escape:
        this->parentWidget()->parentWidget()->close();
        break;
    default:
        break;
    }

    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->orientation() == Qt::Vertical)
    {
        if(event->delta() > 0)
            camera->zoomIn();
        else
            camera->zoomOut();
    }
    else
    {
        if(event->delta() > 0)
            camera->right();
        else
            camera->left();
    }

    event->accept();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug() << "mouseMoveEvent"<<endl;
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

//    qDebug() << "lastPos.x: "<< lastPos.x() << " lastPost.y: " << lastPos.y() <<endl;
//    qDebug() << "event->x: "<< event->x() << " event->y: " << event->y() << endl;
//    qDebug() << "dx: "<< dx << " dy: " << dy << endl;
//    qDebug() << "===========================================" << endl;

    if (event->buttons() & Qt::LeftButton) {
        camera->move(dx, dy);
        //setXRotation(xRot + 8 * dy);
        //setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        //setXRotation(xRot + 8 * dy);
        //setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void GLWidget::startGL()
{
    isStop = false;
    timer->start(30);
}

void GLWidget::stopGL()
{
    isStop = true;
    timer->stop();
}

QPointF GLWidget::getGeoPosition(QPoint point)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)point.x();
    winY = (float)viewport[3] - (float)point.y();
    glReadPixels(point.x(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return QPointF(posX / 100.0, posY / 100.0);
}

void GLWidget::centerMap()
{
    camera->center();
}

void GLWidget::setMap(map<unsigned long int,Way*>* ways)
{
    allWays = ways;
}

void GLWidget::setPath(vector<WaySegment*> waySegments)
{
    path = waySegments;
}

void GLWidget::drawStartPoint(QPointF *startp)
{
    startPoint = startp;
}

void GLWidget::drawEndPoint(QPointF *endp)
{
    endPoint = endp;
}

void GLWidget::deleteStartPoint()
{
    startPoint = 0;
}

void GLWidget::deleteEndPoint()
{
    endPoint = 0;
}

void GLWidget::deletePath()
{
    path.clear();
}

void GLWidget::setPOIs(vector<POIPoint*> pois)
{
    glPOIPoints.clear();
    for(int i = 0; i < pois.size(); i++)
    {
        GLPOIPoint* poiPoint = new GLPOIPoint();
        poiPoint->point = pois[i];
        //qDebug() << "Icon Path: " << poiPoint->point->getIconPath();
        //qDebug() << "Icon Path: " << QString::fromStdString(poiPoint->point->getIconPath());
        if(isGLInitialized)
            poiPoint->texture = bindTexture(QPixmap(QString::fromStdString(poiPoint->point->getIconPath())), GL_TEXTURE_2D);
        poiPoint->setupVertices();
        glPOIPoints.push_back(poiPoint);
    }
}

void GLWidget::deletePOIs()
{
    glPOIPoints.clear();
}
