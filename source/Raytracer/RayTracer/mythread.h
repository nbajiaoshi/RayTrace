#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "camera.h"

class MyThread : public QThread
{
    Q_OBJECT
protected:
    Vec3f ** _color;
    Vec3f _eye;
    Vec3f _UpperLeft;
    Vec3f _dx;
    Vec3f _dy;
    double aperture_radius;
    int _w,_h;
    int _id;
    Scene& scene;
    virtual void run();
public:
    MyThread(Vec3f ** c,Vec3f eye,Vec3f UL,Vec3f dx,Vec3f dy,double AR,int w,int h,int i,Scene&);

};
class AAThread : public QThread
{
    Q_OBJECT
protected:
    Vec3f ** _color;
    Vec3f _eye;
    Vec3f _UpperLeft;
    Vec3f _dx;
    Vec3f _dy;
    int _w,_h;
    int _id;
    Scene& scene;
    double ColorDeviation(Vec3f a,Vec3f b);
    bool ShouldDoAA(int x,int y);
    virtual void run();
public:
    AAThread(Vec3f ** c,Vec3f eye,Vec3f UL,Vec3f dx,Vec3f dy,int w,int h,int i,Scene&);

};

#endif // MYTHREAD_H
