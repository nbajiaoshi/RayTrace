#include "mythread.h"
#include "QDebug"
#include "time.h"
#include <time.h>
#include <iostream>
MyThread::MyThread(Vec3f ** c,Vec3f eye,Vec3f UL,Vec3f dx,Vec3f dy,double AR,int w,int h,int i,Scene& s)
    :_color(c),_eye(eye),_UpperLeft(UL),_dx(dx),_dy(dy),aperture_radius(AR),_w(w),_h(h),_id(i),scene(s)
{
    _UpperLeft = (UL - eye) * Focus + eye;
    _dx = dx * Focus;
    _dy = dy * Focus;
}

void MyThread::run(){
    //Scene scene;
    clock_t start, end;
    start = clock();
    double dx,dy;
    Vec3f nowcolor;
    double dis;
    Vec3f IntersectPoint;
    Vec3f EyeToCenter = _UpperLeft + _dx * (_w/2)+ _dy * (_h/2) - _eye;
    double LenEyeDis = EyeToCenter.Len();
    EyeToCenter.Normalize();
    //cout << LenEyeDis << endl;
    for (int y = _h - 1;y >= 0;y--){
        for (int x = _id;x < _w;x+=threadsNum)
            if (DepthFocus){
                _color[x][y] = Vec3f(0,0,0);
                for (int i = 0;i < SampleNum;i++){
                    dx = rand() * 2.0 / RAND_MAX - 1;
                    dy = rand() * 2.0 / RAND_MAX - 1;
                    while (dx * dx + dy * dy > 1){
                        dx = rand() * 2.0 / RAND_MAX - 1;
                        dy = rand() * 2.0 / RAND_MAX - 1;
                    }
                    dx *= aperture_radius;
                    dy *= aperture_radius;
                    _color[x][y] += scene.RayTracer(Line(_eye + _dx*dx+_dy*dy,_UpperLeft + _dx * (x + ((i/3) %3 * 2 + 1.0)/6 - dx) + _dy * (y + (i %3 * 2 + 1.0)/6 - dy)- _eye),IntersectPoint);
                }
                _color[x][y] /= SampleNum;
                /*nowcolor = scene.RayTracer(Line(_eye,_UpperLeft + _dx * (x + 0.5) + _dy * (y + 0.5)- _eye),IntersectPoint);
                double Imagingdis = 1/(Focus - 1/((IntersectPoint - _eye) * EyeToCenter));
                double R = ApertureRound*fabs(Imagingdis - LenEyeDis)/Imagingdis;

                if (R > 0.5){
                    Vec3f dColor =nowcolor * (0.0016/(R * R * Pi));
                    for (int i = -int(R+1);i < R + 1;i++)
                        for (int j = -int(R+1);j < R + 1;j++)
                            for (double ddx = -0.48;ddx<0.5;ddx+=0.04)
                                for (double ddy = -0.48;ddy<0.5;ddy+=0.04)
                                    if (x + i > 0 && x + i < _w && y + j > 0 && y + j < _h && (i + ddx) * (i + ddx) + (j + ddy) * (j + ddy) < R*R){
                                        /*dis = R - sqrt(i*i+j*j) + 0.5;
                                        if (dis > 1)
                                            dis = 1;
                                        dis /= ;
                                        _color[x + i][y + j] += dColor;
                                    }
                }
                else
                    _color[x][y] += nowcolor;*/
            }
            else
                _color[x][y] = scene.RayTracer(Line(_eye,_UpperLeft + _dx * (x + 0.5) + _dy * (y + 0.5)- _eye),IntersectPoint);
        if (ShowProgress)
            qDebug() <<_id << y;

    }
    end = clock();
    qDebug() << _id << "takes" << end - start << "ms";
    return;
}
AAThread::AAThread(Vec3f ** c,Vec3f eye,Vec3f UL,Vec3f dx,Vec3f dy,int w,int h,int i,Scene& s)
    :_color(c),_eye(eye),_UpperLeft(UL),_dx(dx),_dy(dy),_w(w),_h(h),_id(i),scene(s)
{
}
double AAThread::ColorDeviation(Vec3f a,Vec3f b){
    return fabs(a.r - b.r) + fabs(a.g - b.g) + fabs(a.b - b.b);
}

bool AAThread::ShouldDoAA(int x,int y){
    if (x > 0 && ColorDeviation(_color[x][y],_color[x - 1][y]) > MaxColorDeviation)
        return true;
    if (y > 0 && ColorDeviation(_color[x][y],_color[x][y - 1]) > MaxColorDeviation)
        return true;
    if (x < _w - 1 && ColorDeviation(_color[x][y],_color[x + 1][y]) > MaxColorDeviation)
        return true;
    if (y < _h - 1 && ColorDeviation(_color[x][y],_color[x][y + 1]) > MaxColorDeviation)
        return true;
    return false;

}
void AAThread::run(){
    //Scene scene;
    clock_t start, end;
    start = clock();
    Vec3f IntersectPoint;
    for (int y = 0;y < _h;y++){
        for (int x = _id;x < _w;x+=threadsNum){
            if (ShouldDoAA(x,y)) {
                for (double sx = 1.0/6; sx < 1; sx += 1.0/3)
                    for (double sy = 1.0/6; sy < 1; sy += 1.0/3) if (abs(sx - 0.5) + abs(sy - 0.5) > EPS)
                        _color[x][y] += scene.RayTracer(Line(_eye,_UpperLeft + _dx * (x + sx) + _dy * (y + sy)- _eye),IntersectPoint);
                _color[x][y] = _color[x][y] / 9;
            }

        }
        if (ShowProgress)
            qDebug() << "AA"<<_id  << y;
    }
    end = clock();
    qDebug() <<"AA"<< _id << "takes" << end - start << "ms";
    return;
}
