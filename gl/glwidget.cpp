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
    //glBlendFunc (GL_ONE, GL_ONE);
    //glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);

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
            glVertex3f(nodeGeoPosA.x() * 100.0f, nodeGeoPosA.y() * 100.0f, 0.1f);
            glVertex3f(nodeGeoPosB.x() * 100.0f, nodeGeoPosB.y() * 100.0f, 0.1f);
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
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        glVertex3f(startPoint->x() * 100.0f, startPoint->y() * 100.0f, 0.2f);
        glEnd();
    }

    if(endPoint != NULL)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glPointSize(10.0f);
        glBegin(GL_POINTS);
        glVertex3f(endPoint->x() * 100.0f, endPoint->y() * 100.0f, 0.25f);
        glEnd();
    }


    drawPOIPoints();


    glPopMatrix();
}

void GLWidget::drawPOIPoints()
{
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glEnable(GL_TEXTURE_2D);
    for(int i = 0; i < glPOIPoints.size(); i++)
    {
        if(!glPOIPoints[i]->texture)
            continue;
        glLoadName(i);
        glVertexPointer(3, GL_FLOAT, 0, glPOIPoints[i]->vertices.constData());
        glTexCoordPointer(2, GL_FLOAT, 0, glPOIPoints[i]->textureCoordinates.constData());
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, glPOIPoints[i]->texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GLWidget::scalePOIPoints(float scale)
{
    for(int i = 0; i < glPOIPoints.size(); i++)
        glPOIPoints[i]->setupVertices(scale);
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

        scalePOIPoints((10.0f/camera->zoomLevel));
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

    GLuint	buffer[512];										// Set Up A Selection Buffer
    GLint	hits  = 0;

    // The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>
    GLint	viewport[4];

    // This Sets The Array <viewport> To The Size And Location Of The Screen Relative To The Window
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(512, buffer);								// Tell OpenGL To Use Our Array For Selection

    // Puts OpenGL In Selection Mode. Nothing Will Be Drawn.  Object ID's and Extents Are Stored In The Buffer.
    (void) glRenderMode(GL_SELECT);

    glInitNames();												// Initializes The Name Stack
    glPushName(0);												// Push 0 (At Least One Entry) Onto The Stack

    glMatrixMode(GL_PROJECTION);								// Selects The Projection Matrix
    glPushMatrix();												// Push The Projection Matrix
    glLoadIdentity();											// Resets The Matrix

    // This Creates A Matrix That Will Zoom Up To A Small Portion Of The Screen, Where The Mouse Is.
    gluPickMatrix((GLdouble)event->pos().x(), (GLdouble) (viewport[3] - event->pos().y()), 1.0f, 1.0f, viewport);

    glOrtho(camera->planeLeft,
            camera->planeRight,
            camera->planeBottom,
            camera->planeTop,
            camera->planeNear,
            camera->planeFar);

    glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
    drawPOIPoints();											// Render The Targets To The Selection Buffer
    glMatrixMode(GL_PROJECTION);								// Select The Projection Matrix
    glPopMatrix();												// Pop The Projection Matrix
    glMatrixMode(GL_MODELVIEW);									// Select The Modelview Matrix
    hits=glRenderMode(GL_RENDER);								// Switch To Render Mode, Find Out How Many

    if (hits > 0)
    {
        //qDebug() << "HIT!";
        int	choose = buffer[3];									// Make Our Selection The First Object
        int depth = buffer[1];									// Store How Far Away It Is

        for (int loop = 1; loop < hits; loop++)					// Loop Through All The Detected Hits
        {
            // If This Object Is Closer To Us Than The One We Have Selected
            if (buffer[loop*4+1] < GLuint(depth))
            {
                choose = buffer[loop*4+3];						// Select The Closer Object
                depth = buffer[loop*4+1];						// Store How Far Away It Is
            }
        }

        emit poiClicked(glPOIPoints[choose]->point);

    }


    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
        camera->move(dx, dy);

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
