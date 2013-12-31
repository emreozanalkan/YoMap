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

    connect( ui->pushButton, SIGNAL(released()), this, SLOT(handleButtonGo()));
    connect( ui->pushButtonSwap, SIGNAL(released()), this, SLOT(handleButtonSwap()));

    connect( ui->comboBoxCatA, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryA(int)));
    connect( ui->comboBoxPOIA, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedPOIA(int)));

    connect( ui->comboBoxCatB, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryB(int)));
    connect( ui->comboBoxPOIB, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedPOIB(int)));

    connect( ui->pushButton_2, SIGNAL(released()), this, SLOT(handleButtonGo_Radius()));

    connect( ui->comboBoxCatA_2, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryA_Radius(int)));
    connect( ui->comboBoxPOIA_2, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedPOIA_Radius(int)));

    connect( ui->comboBoxCatB_2, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSelectedCategoryB_Radius(int)));

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), ui->spinBox, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(setMaximumDistance(int)));

    //Category Combo Box filling
    map<unsigned int,POICategory *> *categories = logic.getCategoryCatalog();


    //Add all categories
    for(map<unsigned int,POICategory *>::iterator it = categories->begin();it!=categories->end();it++){
       ui->comboBoxCatA->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));
       ui->comboBoxCatB->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));

       ui->comboBoxCatA_2->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));
       ui->comboBoxCatB_2->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));

    }

    ui->comboBoxCatA->model()->sort(0);
    ui->comboBoxCatB->model()->sort(0);

    ui->comboBoxCatA_2->model()->sort(0);
    ui->comboBoxCatB_2->model()->sort(0);

    //Add mock items
    ui->comboBoxCatA->insertItem(0,QString("Please select a category"));
    ui->comboBoxCatA->setCurrentIndex(0);

    ui->comboBoxCatB->insertItem(0,QString("Please select a category"));
    ui->comboBoxCatB->setCurrentIndex(0);

    //Add mock items
    ui->comboBoxCatA_2->insertItem(0,QString("Please select a category"));
    ui->comboBoxCatA_2->setCurrentIndex(0);

    ui->comboBoxCatB_2->insertItem(0,QString("Please select a category"));
    ui->comboBoxCatB_2->setCurrentIndex(0);

    //Set up inputs
    ui->lineEditLatA->setValidator( new QDoubleValidator(0, 100, 7, this) );
    ui->lineEditLonA->setValidator( new QDoubleValidator(0, 100, 7, this) );

    ui->lineEditLatB->setValidator( new QDoubleValidator(0, 100, 7, this) );
    ui->lineEditLonB->setValidator( new QDoubleValidator(0, 100, 7, this) );

    //connect( ui->centralWidget, SIGNAL(resizeEvent()), this, SLOT(handleResize()));

    //Set up the map
    connect(ui->widget, SIGNAL(poiClicked(POIPoint*, QMouseEvent*)), this, SLOT(poiClicked(POIPoint*, QMouseEvent*)));

    ui->widget->setMap(logic.getAllWays());
    ui->widget->setPOIs(logic.getPOIPointsInCategories());
    ui->widget->startGL();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButtonSwap()
{
        int temp_poi_b = ui->comboBoxPOIB->currentIndex();
        int temp_cat_b = ui->comboBoxCatB->currentIndex();
        QString temp_x_a = ui->lineEditLatA->text();
        QString temp_y_a = ui->lineEditLonA->text();
        QString temp_x_b = ui->lineEditLatB->text();
        QString temp_y_b = ui->lineEditLonB->text();

        ui->comboBoxCatB->setCurrentIndex(ui->comboBoxCatA->currentIndex());
        ui->comboBoxPOIB->setCurrentIndex(ui->comboBoxPOIA->currentIndex());

        ui->comboBoxCatA->setCurrentIndex(temp_cat_b);
        ui->comboBoxPOIA->setCurrentIndex(temp_poi_b);

    ui->lineEditLatA->setText(temp_x_b);
    ui->lineEditLatB->setText(temp_x_a);

    ui->lineEditLonA->setText(temp_y_b);
    ui->lineEditLonB->setText(temp_y_a);

    QPointF temp_point=startPoint;
    startPoint = endPoint;
    endPoint = temp_point;

    //TODO switch the combobox inputs



    ui->widget->deletePath();
}

void MainWindow::handleButtonGo()
{
    Path best_path;
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

    int found = logic.getShortestPath( startPoint, endPoint, mode, best_path);

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

        ui->plainTextEditOutput->setPlainText("Estimated time: " + logic.TimetoSting(logic.getPathTime(best_path,mode)));

        ui->widget->setPath(best_path.segments[0]->segments);
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

            if (ui->tabWidget->currentIndex() != 1){
            menu.addSeparator();

            QAction* actionEnd = new QAction("Set as End Point", this);
            menu.addAction(actionEnd);
            connect(actionEnd, SIGNAL(triggered()), this, SLOT(setEndPoint()));
            }

            connect(actionStart, SIGNAL(triggered()), this, SLOT(setStartPoint()));


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

void MainWindow::deleteStartPoint()
{
   ui->lineEditLatA->setText("");
   ui->lineEditLonA->setText("");

   ui->widget->deleteStartPoint();
   ui->widget->deletePath();
}

void MainWindow::deleteStartPoint_Radius()
{
   ui->lineEditLatA_2->setText("");
   ui->lineEditLonA_2->setText("");

   ui->widget->deleteStartPoint();
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

void MainWindow::deleteEndPoint()
{
    ui->lineEditLatB->setText("");
    ui->lineEditLonB->setText("");

    ui->widget->deleteEndPoint();
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
    //if selected item is not "Please select a category"
    if(index>0){
        if (ui->comboBoxPOIA->count()) ui->comboBoxPOIA->clear();

        POICategory *data = (POICategory *)ui->comboBoxCatA->itemData(index).value<void *>();
        for(vector<POIPoint *>::iterator it = data->getPOIPointsBegin();it!= data->getPOIPointsEnd();it++){
            //qDebug<< ((*it)->getName());
           ui->comboBoxPOIA->addItem(QString::fromStdString((*it)->getName()),qVariantFromValue((void*)(*it)));
        }

        ui->comboBoxPOIA->model()->sort(0);
        //Add mock POI option
        ui->comboBoxPOIA->insertItem(0,"Please select a POI");
        ui->comboBoxPOIA->setCurrentIndex(0);
    }
    else{
        //deleteStartPoint();
        if (ui->comboBoxPOIA->count()) ui->comboBoxPOIA->clear();
        ui->comboBoxPOIA->insertItem(0,"Please select a POI");
        ui->comboBoxPOIA->setCurrentIndex(0);
    }
}

void MainWindow::handleSelectedPOIA(int index)
{
    //Selected item has to be different from "Please select..."
    if (index>0)
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
    else{
        //Clear the selected point
        deleteStartPoint();
    }
}

void MainWindow::handleSelectedCategoryB(int index)
{
    if(index>0){
        if (ui->comboBoxPOIB->count()) ui->comboBoxPOIB->clear();

        POICategory *data = (POICategory *)ui->comboBoxCatB->itemData(index).value<void *>();

        for(vector<POIPoint *>::iterator it = data->getPOIPointsBegin();it!= data->getPOIPointsEnd();it++){
            //qDebug<< ((*it)->getName());
           ui->comboBoxPOIB->addItem(QString::fromStdString((*it)->getName()),qVariantFromValue((void*)(*it)));
        }
        ui->comboBoxPOIB->model()->sort(0);
        ui->comboBoxPOIB->insertItem(0,"Please select a POI");
        ui->comboBoxPOIB->setCurrentIndex(0);
    }
    else{
       // deleteEndPoint();
        if (ui->comboBoxPOIB->count()) ui->comboBoxPOIB->clear();
        ui->comboBoxPOIB->insertItem(0,"Please select a POI");
        ui->comboBoxPOIB->setCurrentIndex(0);
    }

}

void MainWindow::handleSelectedPOIB(int index)
{
    //Selected item has to be different from "Please select..."
    if (index>0)
    {
    POIPoint *data = (POIPoint *)ui->comboBoxPOIB->itemData(index).value<void *>();

    QString StrLon, StrLat;

    StrLat.setNum(data->getGeoPosition().y());
    StrLon.setNum(data->getGeoPosition().x());

    endPoint.setX(data->getGeoPosition().x());
    endPoint.setY(data->getGeoPosition().y());

    ui->lineEditLatB->setText(StrLat);
    ui->lineEditLonB->setText(StrLon);

    ui->widget->deleteEndPoint();
    ui->widget->drawEndPoint(&endPoint);

    ui->widget->deletePath();
    }
    else{
        //Clear the selected point
        deleteEndPoint();
    }
}

void MainWindow::handleSelectedCategoryA_Radius(int index){
    //Selected option is different from "please select"
    qDebug() << "handleSelectedCategoryA_Radius: "<<index << endl;
    if(index>0){
        deleteStartPoint_Radius();
        if (ui->comboBoxPOIA_2->count()) ui->comboBoxPOIA_2->clear();

        POICategory *data = (POICategory *)ui->comboBoxCatA_2->itemData(index).value<void *>();
        for(vector<POIPoint *>::iterator it = data->getPOIPointsBegin();it!= data->getPOIPointsEnd();it++){
           ui->comboBoxPOIA_2->addItem(QString::fromStdString((*it)->getName()),qVariantFromValue((void*)(*it)));
        }
        ui->comboBoxPOIA_2->model()->sort(0);
        ui->comboBoxPOIA_2->insertItem(0,"Please select a POI");
        ui->comboBoxPOIA_2->setCurrentIndex(0);
    }
    else{

        if (ui->comboBoxPOIA_2->count()) ui->comboBoxPOIA_2->clear();
        ui->comboBoxPOIA_2->insertItem(0,"Please select a POI");
       // ui->comboBoxPOIA_2->setCurrentIndex(0);
    }
}


void MainWindow::handleSelectedPOIA_Radius(int index){
    //Selected option is different from "please select"
            qDebug() <<"handleSelectedPOIA_Radius "<< index << endl;
    if (index>0)
    {
        POIPoint *data = (POIPoint *)ui->comboBoxPOIA_2->itemData(index).value<void *>();

        QString StrLon, StrLat;

        StrLat.setNum(data->getGeoPosition().y());
        StrLon.setNum(data->getGeoPosition().x());

        startPoint.setX(data->getGeoPosition().x());
        startPoint.setY(data->getGeoPosition().y());

        ui->lineEditLatA_2->setText(StrLat);
        ui->lineEditLonA_2->setText(StrLon);

        ui->widget->deleteStartPoint();
        ui->widget->drawStartPoint(&startPoint);

        ui->widget->deletePath();
    }
    else{
        deleteStartPoint_Radius();

    }
}

void MainWindow::handleSelectedCategoryB_Radius(int index){
    if(index>0){
        endCategory = (POICategory *)ui->comboBoxCatB_2->itemData(index).value<void *>();
    }
    else{
        endCategory = NULL;
    }
}

void MainWindow::handleButtonGo_Radius(){
    QMessageBox msgBox;
    int mode;
    set<Path*,ComparePaths> all_paths;
    //maxDistance = 1;

    ui->widget->deletePath();

    startPoint.setX(ui->lineEditLonA_2->text().toFloat());
    startPoint.setY(ui->lineEditLatA_2->text().toFloat());
    //TODO validator? error!

    //Validate endCategory
    if(endCategory==NULL){
        msgBox.setText("Please select an end category.");
        msgBox.exec();
        return;
    }

    //Set transport mode
    if (ui->radioButtonDriving_2->isChecked()) mode = 0; //driving 0
    else
    if (ui->radioButtonWalking_2->isChecked()) mode = 1; //walking 1

    //search
    int found = logic.getShortestPathsInRadius( startPoint, endCategory, maxDistance, mode, all_paths);



    if (found==1) {
       msgBox.setText("Sorry, can't calculate the path. Starting point out of bound!");
       msgBox.exec();
    } else
    if (found==3) {
       msgBox.setText("Sorry, path not found.");
       msgBox.exec();
    } else
    if (found==0)
    {
        //text output
        //QString timeOutput;
        //qDebug() << " " << Distance <<" "<< Time <<endl;
         //setPlainText();
        //timeOutput << "";
        //ui->plainTextEditOutput->clear();
        //ui->plainTextEditOutput->appendPlainText(output);

        //QString StrDistance = setNum(StrDistance);

        qDebug() << "Path found" << endl;
        //ui->plainTextEditOutput_2->setPlainText("Estimated time: " + logic.TimetoSting(Time));

        //ui->widget->setPath(all_paths);
    }
}

void MainWindow::setMaximumDistance(int maxDistance_)
{
    maxDistance = maxDistance_ / 1000.;
}

void MainWindow::poiClicked(POIPoint* poiPoint, QMouseEvent* event)
{
    qDebug() << "POI Name: " << poiPoint->getName().c_str();

    qDebug() << "POI Event in GLWidget => x: " << event->x() << " y: " <<event->y();

    QPoint pointInOksanaWindow = ui->widget->mapTo(this, event->pos());

    qDebug() << "POI Event Position according to Oksana's Window: " << pointInOksanaWindow;
}
