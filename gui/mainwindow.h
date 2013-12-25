#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "logic.h"
#include <QMainWindow>
#include "db/database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setLogic(Logic &l);
private:
    Ui::MainWindow *ui;
    Logic l;
    QPoint lastRightClickPoint;
    QPointF startPoint, endPoint;
    Database *db;

protected:
    //void resizeEvent ( QResizeEvent * event );
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void handleButtonGo();
    void setStartPoint();
    void setEndPoint();
};

#endif // MAINWINDOW_H
