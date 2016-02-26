#ifndef VEC4F_H
#define VEC4F_H
#include "Vec3f.h"
#include <string.h>
class Matrix4f;
class Vec4f
{
public:
    Vec4f(double aa,double bb,double cc,double dd):a(aa),b(bb),c(cc),d(dd){}
    Vec4f(Vec3f v):a(v.x),b(v.y),c(v.z),d(1){}
    double operator *(const Vec4f& v) const;
    Vec4f operator *(const Matrix4f& m) const;
protected:
    union
    {
        struct
        { double _p[4]; };
        struct
        { double a,b,c,d; };
    };

};
class Matrix4f
{
public:
    Matrix4f(const Vec3f& N,const Vec3f& p);
    Vec4f operator *(const Vec4f& v) const;
    void operator +=(const Matrix4f& m);
    Matrix4f operator +(const Matrix4f& m) const;
    void operator -=(const Matrix4f& m);
    Matrix4f(){memset(_m,0,sizeof(_m));}
    double _m[4][4];
protected:

};

#endif // VEC4F_H
