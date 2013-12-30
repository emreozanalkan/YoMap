#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "logic.h"
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Logic logic;
    QPoint lastRightClickPoint;
    QPointF startPoint, endPoint; //put to Logic??
    POICategory* endCategory;
    float maxDistance;


protected:
    //void resizeEvent ( QResizeEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
private slots:
        //two points
    void handleButtonGo();
    void handleButtonSwap();
    void setStartPoint();
    void setEndPoint();
    void deleteStartPoint();
    void deleteEndPoint();
    void handleSelectedCategoryA(int);
    void handleSelectedCategoryB(int);
    void handleSelectedPOIA(int);
    void handleSelectedPOIB(int);
        //radius
    void handleSelectedCategoryA_Radius(int);
    void handleSelectedPOIA_Radius(int);
    void handleSelectedCategoryB_Radius(int);
    void handleButtonGo_Radius();
    void deleteStartPoint_Radius();
    void poiClicked(POIPoint*);
};

#endif // MAINWINDOW_H
