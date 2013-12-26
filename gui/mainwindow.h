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
    QPointF startPoint, endPoint;


protected:
    //void resizeEvent ( QResizeEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void handleButtonGo();
    void handleButtonSwap();
    void setStartPoint();
    void setEndPoint();
    void handleSelectedCategoryA(int index);
    void handleSelectedPOIA(int);
};

#endif // MAINWINDOW_H
