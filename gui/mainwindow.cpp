#include <QDebug>
#include <QMenu>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    if(logic.checkIfWaysDBIsBuild() && logic.checkIfWaysDBIsBuild()){
        //HANDLE OF DB DOESNT GET BUILD
    }

    ui->setupUi(this);

    //connect( ui->centralWidget, SIGNAL(resizeEvent()), this, SLOT(handleResize()));   
    connect( ui->pushButton, SIGNAL(released()), this, SLOT(handleButtonGo()));
    connect( ui->pushButtonSwap, SIGNAL(released()), this, SLOT(handleButtonSwap()));

    connect( ui->comboBoxCatA, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryA(int)));
    connect( ui->comboBoxPOIA, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedPOIA(int)));

    connect( ui->comboBoxCatB, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryB(int)));
    connect( ui->comboBoxPOIB, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedPOIB(int)));

    connect(ui->widget, SIGNAL(poiClicked(POIPoint*)), this, SLOT(poiClicked(POIPoint*)));

    //Category Combo Box filling
    map<unsigned int,POICategory *> *categories = logic.getCategoryCatalog();

    for(map<unsigned int,POICategory *>::iterator it = categories->begin();it!=categories->end();it++){
       ui->comboBoxCatA->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));
       ui->comboBoxCatB->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));
    }

    ui->comboBoxCatA->model()->sort(0);
    ui->comboBoxCatB->model()->sort(0);

    //Set up the map
    ui->widget->setMap(logic.getAllWays());
    ui->widget->setPOIs(logic.getPOIPointsInCategories());
    ui->widget->startGL();

    //Set up inputs
    ui->lineEditLatA->setValidator( new QDoubleValidator(0, 100, 7, this) );
    ui->lineEditLonA->setValidator( new QDoubleValidator(0, 100, 7, this) );

    ui->lineEditLatB->setValidator( new QDoubleValidator(0, 100, 7, this) );
    ui->lineEditLonB->setValidator( new QDoubleValidator(0, 100, 7, this) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButtonSwap()
{
    QString temp;
    temp = ui->lineEditLatA->text();
    ui->lineEditLatA->setText(ui->lineEditLatB->text());
    ui->lineEditLatB->setText(temp);

    temp = ui->lineEditLonA->text();
    ui->lineEditLonA->setText(ui->lineEditLonB->text());
    ui->lineEditLonB->setText(temp);

    QPointF temp_point=startPoint;
    startPoint = endPoint;
    endPoint = temp_point;

    //TODO switch the combobox inputs

    ui->widget->deletePath();
}

void MainWindow::handleButtonGo()
{

    float Time, Distance;
    vector<WaySegment*> Path;
    int mode;

    ui->widget->deletePath();

    startPoint.setX(ui->lineEditLonA->text().toFloat());
    startPoint.setY(ui->lineEditLatA->text().toFloat());

    endPoint.setX(ui->lineEditLonB->text().toFloat());
    endPoint.setY(ui->lineEditLatB->text().toFloat());
    //TODO validator? error!

    if (ui->radioButtonDriving->isChecked()) mode = 0; //driving 0
    else
    if (ui->radioButtonWalking->isChecked()) mode = 1; //walking 1

    int found = logic.getShortestPath( startPoint, endPoint, mode, Path, Distance, Time);

    QMessageBox msgBox;

    if (found==1) {
       msgBox.setText("Sorry, can't calculate the path. Starting point out of bound!");
       msgBox.exec();
    } else
    if (found==2) {
       msgBox.setText("Sorry, can't calculate the path. Ending point out of bound!");
       msgBox.exec();
    } else
    if (found==3) {
       msgBox.setText("Sorry, path not found.");
       msgBox.exec();
    } else
    if (found==0)
    {
        //text output
        QString timeOutput;
        //qDebug() << " " << Distance <<" "<< Time <<endl;
         //setPlainText();
        //timeOutput << "";
        //ui->plainTextEditOutput->clear();
        //ui->plainTextEditOutput->appendPlainText(output);

        //QString StrDistance = setNum(StrDistance);


        ui->plainTextEditOutput->setPlainText("Estimated time: " + logic.TimetoSting(Time));

        ui->widget->setPath(Path);
    }
}



void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::RightButton)
    {
//        qDebug() <<"ui widget rect: "<< ui->widget->rect();

//        qDebug() << "widget pos x: " <<ui->widget->pos().x();
//        qDebug() << "widget pos y: " <<ui->widget->pos().y();

//        qDebug() << "event position: "<<event->pos();

        QRect *widgetRect = new QRect(ui->widget->pos().x(), ui->widget->pos().y(), ui->widget->rect().width(), ui->widget->rect().height());

        if(widgetRect->contains(event->pos().x(), event->pos().y()))
        {
            lastRightClickPoint = event->pos();
            QMenu menu;

            QAction* actionStart = new QAction("Set as Start Point", this);
            menu.addAction(actionStart);

            menu.addSeparator();

            QAction* actionEnd = new QAction("Set as End Point", this);
            menu.addAction(actionEnd);

            connect(actionStart, SIGNAL(triggered()), this, SLOT(setStartPoint()));
            connect(actionEnd, SIGNAL(triggered()), this, SLOT(setEndPoint()));

            menu.exec(mapToGlobal(event->pos()));
//            qDebug() <<"signal sent"<<endl;
        }

    }

    QMainWindow::mouseReleaseEvent(event);

}

void MainWindow::setStartPoint()
{
   startPoint = ui->widget->getGeoPosition(lastRightClickPoint - ui->widget->pos());

   QString StrLon, StrLat;

   StrLat.setNum(startPoint.y());
   StrLon.setNum(startPoint.x());

   ui->lineEditLatA->setText(StrLat);
   ui->lineEditLonA->setText(StrLon);

   ui->widget->deleteStartPoint();
   ui->widget->drawStartPoint(&startPoint);

   ui->widget->deletePath();

}

void MainWindow::setEndPoint()
{
    endPoint = ui->widget->getGeoPosition(lastRightClickPoint - ui->widget->pos());

    QString StrLon, StrLat;

    StrLat.setNum(endPoint.y());
    StrLon.setNum(endPoint.x());

    ui->lineEditLatB->setText(StrLat);
    ui->lineEditLonB->setText(StrLon);

    ui->widget->deleteEndPoint();
    ui->widget->drawEndPoint(&endPoint);

    ui->widget->deletePath();
}


/*void MainWindow::resizeEvent ( QResizeEvent * event )
{
    //difference_h = event->oldSize().height() - event->size().height();
    //difference_w = event->oldSize().width() - event->size().width();

   // ui->widget->resize();

    std::cout << "ResizeEvent" << std::endl;
  //this->graphicsView->fitInView(this->BlackItem);
}*/

void MainWindow::handleSelectedCategoryA(int index)
{

    if (ui->comboBoxPOIA->count()) ui->comboBoxPOIA->clear();
    POICategory *data = (POICategory *)ui->comboBoxCatA->itemData(index).value<void *>();

   // map<unsigned int,POICategory *> *categories = logic.getCategoryCatalog();

    for(vector<POIPoint *>::iterator it = data->getPOIPointsBegin();it!= data->getPOIPointsEnd();it++){
        //qDebug<< ((*it)->getName());
       ui->comboBoxPOIA->addItem(QString::fromStdString((*it)->getName()),qVariantFromValue((void*)(*it)));
    }
    ui->comboBoxPOIA->model()->sort(0);

}

void MainWindow::handleSelectedPOIA(int index)
{
    if (index>=0)
    {
    POIPoint *data = (POIPoint *)ui->comboBoxPOIA->itemData(index).value<void *>();

    QString StrLon, StrLat;

    StrLat.setNum(data->getGeoPosition().y());
    StrLon.setNum(data->getGeoPosition().x());

    startPoint.setX(data->getGeoPosition().x());
    startPoint.setY(data->getGeoPosition().y());

    ui->lineEditLatA->setText(StrLat);
    ui->lineEditLonA->setText(StrLon);

    ui->widget->deleteStartPoint();
    ui->widget->drawStartPoint(&startPoint);

    ui->widget->deletePath();
    }
}

void MainWindow::handleSelectedCategoryB(int index)
{

    if (ui->comboBoxPOIB->count()) ui->comboBoxPOIB->clear();
    POICategory *data = (POICategory *)ui->comboBoxCatB->itemData(index).value<void *>();

   // map<unsigned int,POICategory *> *categories = logic.getCategoryCatalog();

    for(vector<POIPoint *>::iterator it = data->getPOIPointsBegin();it!= data->getPOIPointsEnd();it++){
        //qDebug<< ((*it)->getName());
       ui->comboBoxPOIB->addItem(QString::fromStdString((*it)->getName()),qVariantFromValue((void*)(*it)));
    }
    ui->comboBoxPOIB->model()->sort(0);

}

void MainWindow::handleSelectedPOIB(int index)
{
    if (index>=0)
    {
    POIPoint *data = (POIPoint *)ui->comboBoxPOIA->itemData(index).value<void *>();

    QString StrLon, StrLat;

    StrLat.setNum(data->getGeoPosition().y());
    StrLon.setNum(data->getGeoPosition().x());

    startPoint.setX(data->getGeoPosition().x());
    startPoint.setY(data->getGeoPosition().y());

    ui->lineEditLatB->setText(StrLat);
    ui->lineEditLonB->setText(StrLon);

    ui->widget->deleteEndPoint();
    ui->widget->drawEndPoint(&endPoint);

    ui->widget->deletePath();
    }
}

void MainWindow::poiClicked(POIPoint* poiPoint)
{
    //qDebug() << "POI Point Name: " << poiPoint->getName();
    qDebug() << "POI Point Name: " << poiPoint->getName().c_str();
}
