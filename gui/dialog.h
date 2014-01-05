#ifndef DIALOG_H
#define DIALOG_H
#include "db/poi/poipoint.h"
#include <QDialog>
//#include "mainwindow.h"
//class MainWindow;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
//friend class MainWindow;
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void SetCurrentPoint(POIPoint *, map<unsigned int, POICategory *> *categories);
    void CreatePOI(map<unsigned int, POICategory *> *categories);
private:
    Ui::Dialog *ui;

    POIPoint* point;
    map<unsigned int,POICategory *> *categories;

    void clearBindedData();
public slots:
    void acceptedEdit();
    void acceptedPOI();
signals:
    void poiFinishedEditing(POIPoint*);
    void poiCreated(POIPoint*);

};

#endif // DIALOG_H
