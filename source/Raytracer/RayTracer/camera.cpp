#include "camera.h"
#include "qdebug.h"
#include "mythread.h"
#include <unistd.h>
Camera::Camera(Vec3f eye,Vec3f center,Vec3f dx,Vec3f dy,double AR,int w,int h,Scene* s):
    _eye(eye),_UpperLeft(center - dx * (w/2) - dy * (h/2)),_w(w),_h(h),_scene(s),_dx(dx),_dy(dy),aperture_radius(AR / dx.Len())
{
    _color = new Vec3f*[_w];
    for (int i = 0; i < _w;i++)
        _color[i] = new  Vec3f[_h];
}
Vec3f** Camera:: TakePicture()
{
    MyThread* threads[threadsNum];
    for (int i = 0;i < threadsNum;i++)
        threads[i] = new MyThread(_color,_eye,_UpperLeft,_dx,_dy,aperture_radius,_w,_h,i,*_scene);
    for (int i = 0;i < threadsNum;i++)
        threads[i]->start();
    bool finish = false;
    uchar imagedata[_h*_w*3];
    while (!finish){
        finish = true;
        for (int i = 0;i < threadsNum;i++)
            if (!threads[i]->isFinished()){
                finish = false;
            }
        for (int y = 0;y< _h;y++)
            for (int x = 0;x< _w;x++)
                for (int i = 0;i < 3;i++)
                    imagedata[(x + y*_w) * 3 + i] = uchar(_color[x][y][i]);
        QImage pic(imagedata,_w,_h,QImage::Format_RGB888);
        pic.save("result.bmp",0,100);
        sleep(WaitingTime);
    }
    if (AA){
        AAThread* AAthreads[threadsNum];
        for (int i = 0;i < threadsNum;i++)
            AAthreads[i] = new AAThread(_color,_eye,_UpperLeft,_dx,_dy,_w,_h,i,*_scene);
        for (int i = 0;i < threadsNum;i++)
            AAthreads[i]->start();
        finish = false;
        while (!finish){
            finish = true;
            for (int i = 0;i < threadsNum;i++)
                if (!AAthreads[i]->isFinished()){
                    finish = false;
                }
            for (int y = 0;y< _h;y++)
                for (int x = 0;x< _w;x++)
                    for (int i = 0;i < 3;i++)
                        imagedata[(x + y*_w) * 3 + i] = uchar(_color[x][y][i]);
            QImage pic(imagedata,_w,_h,QImage::Format_RGB888);
            pic.save("result.bmp",0,100);
            sleep(WaitingTime);
        }
    }
    //QPicture q;

    //image.s
    /*uchar imagedata[_w*_h*3];
    for (int y = 0;y< _h;y++)
        for (int x = 0;x< _w;x++)
            for (int i = 0;i < 3;i++)
                imagedata[(x + y*_w) * 3 + i] = uchar(_color[x][y][i]);
    QImage pic(imagedata,_w,_h,QImage::Format_RGB888);
    qDebug()<<pic.save("result.bmp",0,100);*/
    return _color;
}
