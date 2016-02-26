#include "mainwindow.h"
#include <QApplication>
#include "scene.h"
#include "camera.h"
#include "constant.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Scene scene;
    //Camera camera(Vec3f(5,0,2),Vec3f(-1,0,-0.5),Vec3f(0,0.0078,0),Vec3f(0.003,0,-0.0072),0.08,WEIGHT,HEIGHT,&scene);
    Camera camera(Vec3f(15.6,0,6.5),Vec3f(12,0,5),Vec3f(0,0.0026,0),Vec3f(0.001,0,-0.0024),0.03,WEIGHT,HEIGHT,&scene);
    w.setFixedSize(WEIGHT,HEIGHT);
    w.setpic(camera.TakePicture());
    w.show();
    return a.exec();
}
