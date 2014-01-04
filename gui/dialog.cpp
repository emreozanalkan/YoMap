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

void Dialog::SetCurrentPoint(POIPoint* poiPoint,  map<unsigned int,POICategory *> *categories)
{
    QString strlat, strlon;

    strlat.setNum(poiPoint->getGeoPosition().y());
    strlon.setNum(poiPoint->getGeoPosition().x());

    ui->lineEditEditPOILat->setText(strlat);
    ui->lineEditEditPOILon->setText(strlon);
    ui->lineEditEditPOIName->setText(QString(poiPoint->getName().c_str()));
    //QString(poiPoint->getCategory()->getName().c_str());

    for(map<unsigned int,POICategory *>::iterator it = categories->begin();it!=categories->end();it++){
       ui->comboBoxEditPOICat->addItem(QString::fromStdString(it->second->getName()),qVariantFromValue((void*)(it->second)));
    }
    ui->comboBoxEditPOICat->model()->sort(0);
    //Add mock items
    ui->comboBoxEditPOICat->insertItem(0,QString("Please select a category"));
    //ui->comboBoxEditPOICat->setCurrentIndex(0);
    ui->comboBoxEditPOICat->setCurrentText(poiPoint->getCategory()->getName().c_str());

    connect( ui->buttonBox, SIGNAL(accepted()), this->parentWidget(), SLOT(handleButtonSave()));

}

Dialog::~Dialog()
{
    delete ui;
}
