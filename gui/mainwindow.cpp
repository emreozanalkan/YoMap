#include <QDebug>
#include <QMenu>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //get data for the lists
    //ui->comboBoxCatA->
    //draw the map
    //connect( ui->centralWidget, SIGNAL(resizeEvent()), this, SLOT(handleResize()));
    connect( ui->pushButton, SIGNAL(released()), this, SLOT(handleButtonGo()));

    ui->widget->setMap(logic.getAllWays());
    ui->widget->startGL();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButtonGo()
{
    int mode;
    //ui->lineEditLatA->setValidator( new QDoubleValidator(0, 100, this) );
    //ui->lineEditLonA->setValidator( new QDoubleValidator(0, 100, this) );

    //ui->lineEditLatB->setValidator( new QDoubleValidator(0, 100, this) );
    //ui->lineEditLonB->setValidator( new QDoubleValidator(0, 100, this) );



    if (ui->radioButtonDriving->isChecked()) mode = 0;
    else
    if (ui->radioButtonWalking->isChecked()) mode = 1;


    //TODO: check validity of the lon/lat

    //l.printmsg();
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

    //get geo position
    //ui->widget->getGeoPosition(event->pos() - ui->widget->pos());
    lastRightClickPoint = event->pos();
    QMainWindow::mouseReleaseEvent(event);

}

void MainWindow::setStartPoint()
{
   startPoint = ui->widget->getGeoPosition(lastRightClickPoint - ui->widget->pos());

   QString StrLon, StrLat;

   StrLat.setNum(startPoint.x());
   StrLon.setNum(startPoint.y());

   ui->lineEditLatA->setText(StrLat);
   ui->lineEditLonA->setText(StrLon);

}

void MainWindow::setEndPoint()
{
    endPoint = ui->widget->getGeoPosition(lastRightClickPoint - ui->widget->pos());

    QString StrLon, StrLat;

    StrLat.setNum(endPoint.x());
    StrLon.setNum(endPoint.y());

    ui->lineEditLatB->setText(StrLat);
    ui->lineEditLonB->setText(StrLon);
}


/*void MainWindow::resizeEvent ( QResizeEvent * event )
{
    //difference_h = event->oldSize().height() - event->size().height();
    //difference_w = event->oldSize().width() - event->size().width();

   // ui->widget->resize();

    std::cout << "ResizeEvent" << std::endl;
  //this->graphicsView->fitInView(this->BlackItem);
}*/
