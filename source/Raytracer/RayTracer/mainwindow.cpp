#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("光线追踪"));
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    for (int x = 0; x < size().width(); x++)
        for (int y = 0; y < size().height(); y++) {
           /* if (pic[x][y].b > 255 || pic[x][y].g > 255 || pic[x][y].r > 255 || pic[x][y].b < 0 || pic[x][y].r < 0 || pic[x][y].g < 0)
                qDebug() <<x <<' ' << y<< ' '<< pic[x][y].r << ' ' << pic[x][y].g << ' ' << pic[x][y].b;*/
            painter.setPen(QColor(pic[x][y].r,pic[x][y].g,pic[x][y].b));
            painter.drawPoint(QPoint(x, y));
        }
}
MainWindow::~MainWindow()
{
    delete ui;
}
