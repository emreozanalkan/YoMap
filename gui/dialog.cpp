#include "dialog.h"
#include "ui_dialog.h"
#include "logic.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);


    //Add mock items
    ui->comboBoxEditPOICat->insertItem(0,QString("Please select a category"));
    ui->comboBoxEditPOICat->setCurrentIndex(0);

}

void Dialog::SetCurrentPoint(POIPoint* poiPoint)
{
    QString strlat, strlon;

    strlat.setNum(poiPoint->getGeoPosition().y());
    strlon.setNum(poiPoint->getGeoPosition().x());

    ui->lineEditEditPOILat->setText(strlat);
    ui->lineEditEditPOILon->setText(strlon);
    ui->lineEditEditPOIName->setText(QString(poiPoint->getName().c_str()));
    //QString(poiPoint->getCategory()->getName().c_str());

}

Dialog::~Dialog()
{
    delete ui;
}
