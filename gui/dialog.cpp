#include "dialog.h"
#include "ui_dialog.h"
#include "logic.h"
#include "mainwindow.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    //Add mock items
    ui->comboBoxEditPOICat->insertItem(0,QString("Please select a category"));
    ui->comboBoxEditPOICat->setCurrentIndex(0);

}

void Dialog::clearBindedData()
{
    ui->comboBoxEditPOICat->clear();
    ui->lineEditEditPOIName->clear();
    ui->lineEditEditPOILat->clear();
    ui->lineEditEditPOILon->clear();
    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptedEdit()));
    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptedPOI()));
}

void Dialog::SetCurrentPoint(POIPoint* poiPoint,  map<unsigned int,POICategory *> *categories)
{
    clearBindedData();
    this->point = poiPoint;
    this->categories = categories;

    QString strlat, strlon;

    strlat.setNum(point->getGeoPosition().y());
    strlon.setNum(point->getGeoPosition().x());

    ui->lineEditEditPOILat->setText(strlat);
    ui->lineEditEditPOILon->setText(strlon);
    ui->lineEditEditPOIName->setText(QString(point->getName().c_str()));
    //QString(poiPoint->getCategory()->getName().c_str());

    for(map<unsigned int, POICategory*>::iterator it = this->categories->begin(); it != this->categories->end(); it++)
       ui->comboBoxEditPOICat->addItem(QString::fromStdString(it->second->getName()), qVariantFromValue((void*)(it->second)));

    ui->comboBoxEditPOICat->model()->sort(0);
    //Add mock items
    ui->comboBoxEditPOICat->insertItem(0,QString("Please select a category"));
    //ui->comboBoxEditPOICat->setCurrentIndex(0);
    ui->comboBoxEditPOICat->setCurrentText(point->getCategory()->getName().c_str());

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptedEdit()));

}

void Dialog::CreatePOI(POIPoint* poiPoint, map<unsigned int, POICategory *> *categories, QPointF point)
{
    clearBindedData();
    this->point = poiPoint;
    this->categories = categories;

    QString strlat, strlon;
    strlat.setNum(point.y());
    strlon.setNum(point.x());

    ui->lineEditEditPOILat->setText(strlat);
    ui->lineEditEditPOILon->setText(strlon);

    for(map<unsigned int, POICategory*>::iterator it = this->categories->begin(); it != this->categories->end(); it++)
       ui->comboBoxEditPOICat->addItem(QString::fromStdString(it->second->getName()), qVariantFromValue((void*)(it->second)));

    ui->comboBoxEditPOICat->model()->sort(0);
    //Add mock items
    ui->comboBoxEditPOICat->insertItem(0,QString("Please select a category"));
    ui->comboBoxEditPOICat->setCurrentIndex(0);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptedPOI()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::acceptedEdit()
{
    POICategory *data = (POICategory *)ui->comboBoxEditPOICat->itemData(ui->comboBoxEditPOICat->currentIndex()).value<void *>();

    if (ui->comboBoxEditPOICat->currentIndex()!=0) {
            point->setCategory(data);
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter the category.");
        msgBox.exec();
        return;
    }

    if (!ui->lineEditEditPOIName->text().isEmpty()) {
        point->setName(ui->lineEditEditPOIName->text().toUtf8().constData());
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter the name of the POI.");
        msgBox.exec();
        //return;
    }

    point->setGeoPosition(ui->lineEditEditPOILon->text().toFloat(), ui->lineEditEditPOILat->text().toFloat());

    //(MainWindow *)this->parent()->handleButtonSave(point);
    emit poiFinishedEditing(point);
}

void Dialog::acceptedPOI()
{
    point = new POIPoint(0);
    POICategory *data = (POICategory *)ui->comboBoxEditPOICat->itemData(ui->comboBoxEditPOICat->currentIndex()).value<void *>();

    if (ui->comboBoxEditPOICat->currentIndex()!=0) {
        point->setCategory(data);
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter the category.");
        msgBox.exec();
        return;
    }

    if (!ui->lineEditEditPOIName->text().isEmpty()) {
        point->setName(ui->lineEditEditPOIName->text().toUtf8().constData());
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("Please enter the name of the POI.");
        msgBox.exec();
        //return;
    }

    point->setGeoPosition(ui->lineEditEditPOILon->text().toFloat(), ui->lineEditEditPOILat->text().toFloat());

    //(MainWindow *)this->parent()->handleButtonSave(point);
    emit poiCreated(point);
}
