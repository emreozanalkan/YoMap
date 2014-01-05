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

    //newPath = NULL;

    searchRadius = 0.0f;

    shouldDrawBuildings = false;

    shouldDrawAllWays = true;
    shouldDrawWays1 = true;
    shouldDrawWays2 = true;
    shouldDrawWays3 = true;
    shouldDrawWays4 = true;
    shouldDrawWays5 = true;
    shouldDrawWays6 = true;
    shouldDrawWays7 = true;
    shouldDrawWays8 = true;
    shouldDrawWays9 = true;
}

void GLWidget::initializeGL()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    //glClearColor(0.329412, 0.329412, 0.329412, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
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
    glAlphaFunc(GL_GREATER, 0.2f);

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

void GLWidget::searchRadiusChanged(float radius)
{
    searchRadius = radius;
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camera->x, camera->y, camera->z,
              camera->look_x, camera->look_y, camera->look_z,
              0.0, 1.0, 0.0);

    glPushMatrix();

    drawMapBorder();

    glColor3f(1.0f, 1.0f, 1.0f);

    glLineWidth(1.0f);

    if(allWays != NULL && shouldDrawAllWays)
        drawWays();

    if(shouldDrawBuildings)
        drawBuildings();

    if(!newPath.segments.empty())
        drawPath();

    if(startPoint != NULL)
        drawStartPoint();

    if(endPoint != NULL)
        drawEndPoint();


    if(!glPOIPoints.empty())
        drawPOIPoints();

    drawRadiusSearch();

    glPopMatrix();
}

void GLWidget::drawStartPoint()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3d(startPoint->x() * 100.0, startPoint->y() * 100.0, 0.6);
    glEnd();
}

void GLWidget::drawEndPoint()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex3d(endPoint->x() * 100.0, endPoint->y() * 100.0, 0.7);
    glEnd();
}

void GLWidget::drawWays()
{
    bool drawWay;
    map<unsigned long int, Way*>::iterator wayIt;
    for(wayIt = allWays->begin(); wayIt != allWays->end(); ++wayIt)
    {
        drawWay = true;
        Way* way = ((Way*)(*wayIt).second);
        switch(way->getWayType())
        {
            case ns_way::primary:
                glColor3f(0.8627f, 0.6196f, 0.6196f);
                glLineWidth(5.0);
                if(!shouldDrawWays1) drawWay = false;
                break;
            case ns_way::primary_link:
                glColor3f(0.8627f, 0.6196f, 0.6196f);
                glLineWidth(4.0);
                if(!shouldDrawWays2) drawWay = false;
                break;
            case ns_way::secondary:
            case ns_way::secondary_link:
                glColor3f(0.9726f, 0.8353f, 0.66274f);
                glLineWidth(4.0);
                if(!shouldDrawWays3) drawWay = false;
                break;
            case ns_way::tertiary:
            case ns_way::tertiary_link:
                glColor3f(0.97255f, 0.97255f, 0.72941f);
                glLineWidth(3.0);
                if(!shouldDrawWays4) drawWay = false;
                break;
            case ns_way::footway:
            case ns_way::steps:
                glColor3f(0.8078f, 0.9255f, 0.6588f);
                glLineWidth(1.5);
                if(!shouldDrawWays5) drawWay = false;
                break;
            case ns_way::track:
                glColor3f(1.0f, 0.0f, 1.0f);
                glLineWidth(1.0);
                if(!shouldDrawWays6) drawWay = false;
                break;
            case ns_way::raceway:
                glColor3f(0.37255f, 0.3764f, 0.345098f);
                glLineWidth(2.0);
                if(!shouldDrawWays7) drawWay = false;
                break;
            case ns_way::service:
                glColor3f(0.737255f, 0.560784f, 0.560784f);
                glLineWidth(1.0);
                if(!shouldDrawWays8) drawWay = false;
                break;
            case ns_way::unclassified:
            case ns_way::residential:
            default:
                glColor3f(1.0f, 1.0f, 1.0f);
                glLineWidth(2.0);
                if(!shouldDrawWays9) drawWay = false;
                break;
        }

        if(!drawWay) continue;

        glBegin(GL_LINE_STRIP);
        for (vector<Node*>::iterator nodeIt = way->getNodesBegin(); nodeIt != way->getNodesEnd(); nodeIt++){
            boost_xy_point& nodeGeoPos = (*nodeIt)->getGeoPosition();
            glVertex3d(nodeGeoPos.x() * 100.0, nodeGeoPos.y() * 100.0, 0.2f);
        }
        glEnd();
    }
}

void GLWidget::drawBuildings()
{
    glColor3f(0.74509f, 0.6784f, 0.6784f);
    if(allBuildings != NULL)
    {
        map<unsigned long int, Building*>::iterator buildingIt;
        for(buildingIt = allBuildings->begin(); buildingIt != allBuildings->end(); ++buildingIt)
        {
            Building* building = ((Building*)(*buildingIt).second);

            glBegin(GL_LINE_LOOP);
            for (vector<Node*>::iterator nodeIt = building->getNodesBegin(); nodeIt != building->getNodesEnd(); nodeIt++){
                boost_xy_point& nodeGeoPos = (*nodeIt)->getGeoPosition();
                glVertex3d(nodeGeoPos.x() * 100.0, nodeGeoPos.y() * 100.0, 0.2f);
            }
            glEnd();
        }
    }
}

void GLWidget::drawRadiusSearch()
{
    if(startPoint == NULL)
        return;

    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    double ratio = double(camera->canvasWidth) / double(camera->canvasHeight);

    if( ratio < 1.0)
        ratio = 1.0 / ratio;

//    glBegin(GL_LINE_LOOP);
//    for(int i = 0; i < 360; i++)
//    {
//        double angle = 2 * 3.1415926 * i / 360;
//        double x = cos(angle);
//        double y = sin(angle);
//        glVertex2d(x * ratio + (startPoint->x() * 100.0), y + (startPoint->y() * 100.0));
//    }
//    glEnd();

    int num_segments = 360;
    double theta = 2 * 3.1415926 / double(num_segments);
    double c = cosf(theta);//precalculate the sine and cosine
    double s = sinf(theta);
    double t;

    double x = searchRadius/1.23;//we start at angle = 0
    double y = 0;

    //glColor3f(1.0f, 0.0f, 0.0f);
    //glColor4d(0.975, 0.705, 0.7137, 0.501);
    glColor4d(1.0, 0.0, 0.0, 0.21);
    glLineWidth(5.0f);
    glBegin(GL_POLYGON);
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(x * ratio + (startPoint->x() * 100.0), y + (startPoint->y() * 100.0), 0.29);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();


    if(radiusSearch.empty())
        return;

    float lineWidth = 2.0;
    glColor3f(1.0f, 0.0f, 0.0f);
//    glBegin(GL_LINES);

    int myColor = 0;

    set<Path*,ComparePaths>::iterator it;
    for (it = radiusSearch.begin(); it != radiusSearch.end(); ++it, lineWidth += 2.0)
    {
        Path* path = *it;
        vector<PathSegment*>::iterator it_path = path->getPathSegmentsBegin();
        if(startPoint != NULL && !(*it_path)->isEmpty())
        {
            glBegin(GL_LINES);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3d(startPoint->x() * 100.0, startPoint->y() * 100.0, 0.3);
            WaySegment* waySegment = (*it_path)->segments[0];
            boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
            glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.3);
            glEnd();
        }
        pickOpenGLColor(myColor);
        for(; it_path != path->getPathSegmentsEnd(); it_path++)
        {
            glLineWidth(lineWidth);
            glBegin(GL_LINES);
            vector<WaySegment*>::iterator it_p_seg = (*it_path)->getWaySegmentsBegin();
            for(; it_p_seg != (*it_path)->getWaySegmentsEnd(); it_p_seg++)
            {
                //cout<<(*it_p_seg)->getPointA()->getId()<<" -- "<<(*it_p_seg)->getPointB()->getId()<< "(type: "<<(*it_p_seg)->getWay()->getWayType()<<")"<<endl;
                boost_xy_point& nodeGeoPosA = (*it_p_seg)->getPointA()->getGeoPosition();
                boost_xy_point& nodeGeoPosB = (*it_p_seg)->getPointB()->getGeoPosition();
                //glColor3d((nodeGeoPosA.x() - int(nodeGeoPosA.x())) / colorRandom, (nodeGeoPosA.y() - int(nodeGeoPosA.y())) / colorRandom, (nodeGeoPosB.x() - int(nodeGeoPosB.x()))  / colorRandom);
                glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.3);
                glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.3);
            }
            glEnd();
            --it_p_seg;
            glPointSize(20.0f);
            glBegin(GL_POINTS);
            glVertex3d((*it_p_seg)->getPointB()->getGeoPosition().x() * 100.0, (*it_p_seg)->getPointB()->getGeoPosition().y() * 100.0, 0.55);
            glEnd();
        }
        myColor++;
    }



    glPopMatrix();

}

void GLWidget::drawPath()
{
//    glLineWidth(5.0f);
//    glColor3f(1.0f, 0.0f, 0.0f);
//    glBegin(GL_LINES);
//    if(startPoint != NULL && !path.empty())
//    {
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3d(startPoint->x() * 100.0, startPoint->y() * 100.0, 0.1);
//        WaySegment* waySegment = path[0];
//        boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
//        glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.1);
//    }
//    glColor3f(1.0f, 0.0f, 0.0f);
//    int i;
//    for(i = 0; i < path.size(); i++)
//    {
//        WaySegment* waySegment = path[i];
//        boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
//        boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
//        glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.1);
//        glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.1);
//    }
//    if(endPoint != NULL && !path.empty())
//    {
//        glColor3f(0.0f, 0.0f, 1.0f);
//        WaySegment* waySegment = path[i-1];
//        boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
//        glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.1);
//        glVertex3d(endPoint->x() * 100.0, endPoint->y() * 100.0, 0.1);
//    }
//    glEnd();

    //qDebug() << "there is newPath segments!!!!";
    glLineWidth(5.0f);
    glBegin(GL_LINES);

    vector<PathSegment*>::iterator it_path = newPath.getPathSegmentsBegin();
    if(startPoint != NULL && !(*it_path)->isEmpty())
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3d(startPoint->x() * 100.0, startPoint->y() * 100.0, 0.3);
        WaySegment* waySegment = (*it_path)->segments[0];
        boost_xy_point& nodeGeoPosA = waySegment->getPointA()->getGeoPosition();
        glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.3);
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    int i = 0;
    for(; it_path != newPath.getPathSegmentsEnd(); it_path++)
    {
        i = 0;
        vector<WaySegment*>::iterator it_p_seg = (*it_path)->getWaySegmentsBegin();
        for(; it_p_seg != (*it_path)->getWaySegmentsEnd(); it_p_seg++, i++)
        {
            //cout<<(*it_p_seg)->getPointA()->getId()<<" -- "<<(*it_p_seg)->getPointB()->getId()<< "(type: "<<(*it_p_seg)->getWay()->getWayType()<<")"<<endl;
            boost_xy_point& nodeGeoPosA = (*it_p_seg)->getPointA()->getGeoPosition();
            boost_xy_point& nodeGeoPosB = (*it_p_seg)->getPointB()->getGeoPosition();
            glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.3);
            glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.3);
        }
    }
    --it_path;
    if(endPoint != NULL && !(*it_path)->isEmpty())
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        WaySegment* waySegment = (*it_path)->segments[i-1];
        boost_xy_point& nodeGeoPosB = waySegment->getPointB()->getGeoPosition();
        glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.3);
        glVertex3d(endPoint->x() * 100.0, endPoint->y() * 100.0, 0.3);
    }
    glEnd();



//    if(newPath == NULL)
//        return;

//    glLineWidth(5.0f);
//    glColor3f(1.0f, 0.0f, 0.0f);
//    glBegin(GL_LINES);
//    vector<PathSegment*>::iterator it_path = newPath->getPathSegmentsBegin();
//    for(; it_path != newPath->getPathSegmentsEnd(); it_path++){
//        vector<WaySegment*>::iterator it_p_seg = (*it_path)->getWaySegmentsBegin();
//        for(; it_p_seg != (*it_path)->getWaySegmentsEnd(); it_p_seg++){
//            //cout<<(*it_p_seg)->getPointA()->getId()<<" -- "<<(*it_p_seg)->getPointB()->getId()<< "(type: "<<(*it_p_seg)->getWay()->getWayType()<<")"<<endl;
//            boost_xy_point& nodeGeoPosA = (*it_p_seg)->getPointA()->getGeoPosition();
//            boost_xy_point& nodeGeoPosB = (*it_p_seg)->getPointB()->getGeoPosition();
//            glVertex3d(nodeGeoPosA.x() * 100.0, nodeGeoPosA.y() * 100.0, 0.1);
//            glVertex3d(nodeGeoPosB.x() * 100.0, nodeGeoPosB.y() * 100.0, 0.1);
//        }
//    }
//    glEnd();
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

void GLWidget::scalePOIPoints(double scale)
{
    for(int i = 0; i < glPOIPoints.size(); i++)
        glPOIPoints[i]->setupVertices(scale);
}

void GLWidget::resizeGL(int w, int h)
{
    camera->canvasWidth = w;
    camera->canvasHeight = h;

    camera->setViewingVolume();
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
    case Qt::Key_B:
        shouldDrawBuildings = !shouldDrawBuildings;
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
        break;
    case Qt::Key_1:
        shouldDrawAllWays = !shouldDrawAllWays;
        break;
    case Qt::Key_2:
        shouldDrawWays1 = !shouldDrawWays1;
        break;
    case Qt::Key_3:
        shouldDrawWays2 = !shouldDrawWays2;
        break;
    case Qt::Key_4:
        shouldDrawWays3 = !shouldDrawWays3;
        break;
    case Qt::Key_5:
        shouldDrawWays4 = !shouldDrawWays4;
        break;
    case Qt::Key_6:
        shouldDrawWays5 = !shouldDrawWays5;
        break;
    case Qt::Key_7:
        shouldDrawWays6 = !shouldDrawWays6;
        break;
    case Qt::Key_8:
        shouldDrawWays7 = !shouldDrawWays7;
        break;
    case Qt::Key_9:
        shouldDrawWays8 = !shouldDrawWays8;
        break;
    case Qt::Key_0:
        shouldDrawWays9 = !shouldDrawWays9;
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(event->orientation() == Qt::Vertical)
    {
        if(event->delta() > 0)
            camera->zoomIn();
        else
            camera->zoomOut();

        scalePOIPoints(2.0f / camera->zoomLevel);
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
    if (event->buttons() & Qt::RightButton)
    {

        return;
    }

    this->setCursor(Qt::ClosedHandCursor);

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

    camera->setProjection();

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

        emit poiClicked(glPOIPoints[choose]->point, event);

    }


    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //this->setCursor(Qt::ClosedHandCursor);
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
        camera->move(dx, dy);

    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    this->setCursor(Qt::OpenHandCursor);
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

POIPoint* GLWidget::getIfPOI(QPointF point)
{
    for(int i = 0; i < glPOIPoints.size(); i++)
    {
        GLPOIPoint* glpoint = glPOIPoints[i];
        if(glpoint->isContains(point))
            return glpoint->point;
    }

    return NULL;
}

void GLWidget::centerMap()
{
    camera->center();
}

void GLWidget::setMap(map<unsigned long int,Way*>* ways)
{
    allWays = ways;
}

void GLWidget::setBuildings(map<unsigned long int,Building *>* buildings)
{
    allBuildings = buildings;
}

void GLWidget::setPath(vector<WaySegment*> waySegments)
{
    path = waySegments;
}

void GLWidget::setPath(Path path)
{
    newPath = path;
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
    //newPath = 0;
    //PathAlgorithms::safelyDeletePath(newPath);
    newPath.segments.clear();
    radiusSearch.clear();
}

void GLWidget::setRadiusSearch(set<Path*,ComparePaths> radiusSeearchPaths, float searchRadius)
{
    radiusSearch = radiusSeearchPaths;
    this->searchRadius = searchRadius;
}

void GLWidget::deleteRadiusSearch()
{
    radiusSearch.clear();
    searchRadius = 0.0f;
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

QString GLWidget::pickOpenGLColor(int index)
{
    QString colorText = "";
    index = index % 7;
    switch(index)
    {
        case 0:
            // RED
            glColor3d(1.0, 0.0, 0.0);
            colorText = "RED";
            break;
        case 1:
            // Green
            glColor3d(0.0, 1.0, 0.0);
            colorText = "GREEN";
            break;
        case 2:
            // Blue
            glColor3d(0.0, 0.0, 1.0);
            colorText = "BLUE";
            break;
        case 3:
            // White
            glColor3d(1.0, 1.0, 1.0);
            colorText = "WHITE";
            break;
        case 4:
            // Black
            glColor3d(0.0, 0.0, 0.0);
            colorText = "BLACK";
            break;
        case 5:
            // Yellow
            glColor3d(1.0, 1.0, 0.0);
            colorText = "YELLOW";
            break;
        case 6:
            // Pink (Magenta)
            glColor3d(1.0, 0.0, 1.0);
            colorText = "PINK";
            break;
        default:
            glColor3d(1.0, 1.0, 1.0);
            colorText = "WHITE";
            break;
    }
    return colorText;
}

void GLWidget::drawMapBorder()
{
    glColor3d(0.329412, 0.329412, 0.329412);
    //glColor3d(0.86, 0.86, 0.86);
    glBegin(GL_QUADS);
    glVertex3d(mapMaxBound.x() * 100.0, mapMaxBound.y() * 100.0, 0.0);
    glVertex3d(mapMinBound.x() * 100.0, mapMaxBound.y() * 100.0, 0.0);
    glVertex3d(mapMinBound.x() * 100.0, mapMinBound.y() * 100.0, 0.0);
    glVertex3d(mapMaxBound.x() * 100.0, mapMinBound.y() * 100.0, 0.0);
    glEnd();
}

void GLWidget::setMapBounds(boost_xy_point min, boost_xy_point max)
{
    mapMinBound = min;
    mapMaxBound = max;
}

void GLWidget::setSearchRadius(float search)
{
    searchRadius = search;
}
