#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Vec3f.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setpic(Vec3f** p) {pic = p;}
    ~MainWindow();
protected:
   double finish;
   Vec3f** pic;
   void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
