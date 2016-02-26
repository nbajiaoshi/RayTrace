#ifndef GEOMETRYELEMENT_H
#define GEOMETRYELEMENT_H
#include "Vec3f.h"
#include <math.h>
#include <QImage>
#include <QString>
#include "constant.h"
enum ElementType{
    ball,
    triangle
};
struct Material{
    Vec3f color;
    double Ka,Kd,Ks;
    int Ksn;
    double reflect_rate,refract_rate,refract_n,absorptivity,diffuse_reflectance;
    Vec3f getColor(int x,int y);
    Material():reflect_rate(0),refract_n(0),absorptivity(0),diffuse_reflectance(1){}
    void setGreenBall();
    void setRedBall();
    void setLight();
    void setStone();
    void setSmoothSurface();
    void setNormalPlane();
    void setCube();
    void setWall();

};
struct Line
{
    Line(Vec3f s,Vec3f d):start_point(s),dir(d){dir.Normalize();}
    Vec3f start_point;
    Vec3f dir;
    Material material;
};
class Object
{
public:
    Object():use_texture(false){}
    virtual double getIntersection(Line) = 0;
    virtual Vec3f getColor(Vec3f) = 0;
    virtual Vec3f getN(Vec3f IntersectPoint) = 0;
    Material material;
    void LoadTexture(QString filename,double = 100);
protected:
    bool use_texture;
    QImage* _texture;
    double pixel_amplification_rate;

};

class Ball:public Object
{
public:
    Ball(Vec3f c,double r,Vec3f color = Vec3f(0,255,0));
    virtual Vec3f getColor(Vec3f);
    virtual double getIntersection(Line);
    virtual Vec3f getN(Vec3f IntersectPoint);
protected:
    Vec3f center;
    double radius;
};
class GlassBall:public Ball{
public:
    GlassBall(Vec3f c,double r):Ball(c,r){material.setRedBall();}
};
class TextureBall:public Ball{
public:
    TextureBall(Vec3f c,double r,QString Fn):Ball(c,r){LoadTexture(Fn);material.setCube();}
};
class Plane:public Object
{
public:
    Plane(Vec3f p,Vec3f n);
    Plane(double d,Vec3f n);
    virtual Vec3f getColor(Vec3f);
    virtual double getIntersection(Line);
    virtual Vec3f getN(Vec3f IntersectPoint);
protected:
    double D;
    Vec3f N;
};
class Triangle:public Object
{
public:
    Triangle(Vec3f,Vec3f,Vec3f);
    virtual Vec3f getColor(Vec3f);
    virtual double getIntersection(Line);
    virtual Vec3f getN(Vec3f IntersectPoint);
    double AABB[3][2];
    double S;
protected:
    double Det3(Vec3f,Vec3f,Vec3f);
    Vec3f P[3];
    Vec3f N;
    Vec3f E1,E2;
};

#endif // GEOMETRYELEMENT_H
