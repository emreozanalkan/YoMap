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
    vector<unsigned int> Categories;
    float maxDistance;


protected:
    //void resizeEvent ( QResizeEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void poiClicked(POIPoint*, QMouseEvent*);
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
    void setMaximumDistance(int);
       //itineary
    void handleSelectedCategoryA_Itineary(int);
    void handleSelectedCategoryB_Itineary(int);
    void handleSelectedPOIA_Itineary(int);
    void handleSelectedPOIB_Itineary(int);
    void deleteStartPoint_Itineary();
    void deleteEndPoint_Itineary();
    void handleButtonGo_Itineary();
    //void handleSelectedCategoryMidCat_Radius();
};

#endif // MAINWINDOW_H
