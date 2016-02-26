#include "Vec3f.h"
#include <math.h>
#include <cstring>
Vec3f::Vec3f(void)
{
    memset(_p,0,sizeof(double)*_len);
}

Vec3f::Vec3f(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3f::Vec3f(const Vec3f &v)
{
    memcpy(_p,v._p,sizeof(double)*_len);
}

Vec3f::~Vec3f(void)
{

}

//////////////////////////////////////////////////////////////////////////
// Operators

Vec3f& Vec3f::operator =( const Vec3f& v)
{
    memcpy(_p,v._p,sizeof(double)*_len);
    return (*this);
}

void Vec3f::operator +=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] += v._p[i];
}
void Vec3f::operator +=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] += f;
}

void Vec3f::operator -=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] -= v._p[i];
}
void Vec3f::operator -=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] -= f;
}

void Vec3f::operator *=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] *= v._p[i];
}
void Vec3f::operator *=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] *= f;
}

void Vec3f::operator /=(const Vec3f& v)
{
    for(int i=0;i<_len;i++)
        _p[i] /= v._p[i];
}
void Vec3f::operator /=(double f)
{
    for(int i=0;i<_len;i++)
        _p[i] /= f;
}

Vec3f Vec3f::operator +(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] + v[i];
    return res;
}
Vec3f Vec3f::operator +(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] + f;
    return res;
}

Vec3f Vec3f::operator -(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] - v[i];
    return res;
}
Vec3f Vec3f::operator -(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] - f;
    return res;
}

double Vec3f::operator *(const Vec3f&v) const
{
    double ans = 0;
    for(int i=0;i<_len;i++)
        ans += (*this)[i] * v[i];
    return ans;
}
Vec3f Vec3f::operator *(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] * f;
    return res;
}

Vec3f Vec3f::operator /(const Vec3f&v) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] / v[i];
    return res;
}
Vec3f Vec3f::operator /(double f) const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = (*this)[i] / f;
    return res;
}

Vec3f Vec3f::operator - () const
{
    Vec3f res;
    for(int i=0;i<_len;i++)
        res[i] = -(*this)[i];
    return res;
}
Vec3f Vec3f::operator ^(const Vec3f& v) const{
    return Vec3f(y * v.z - z * v.y,z * v.x - x * v.z,x * v.y - y * v.x);
}
Vec3f Vec3f::operator |(const Vec3f& v) const{
    return Vec3f(x * v.x / 255,y * v.y/255,z * v.z/255);
}
Vec3f Vec3f::mul(const Vec3f&v) const{
    return Vec3f(x * v.x / 255,y * v.y/255,z * v.z/255);
}

//////////////////////////////////////////////////////////////////////////
// Other Methods
void Vec3f::Normalize()
{
    double fSqr = Len();
    if(fSqr>EPS)
        (*this) *= 1.0f/fSqr;
}

double Vec3f::L2Norm_Sqr()
{
    return _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2];
}
double Vec3f::Len()
{
    return sqrt(L2Norm_Sqr());
}
void Vec3f::Fix255(){
    if (r > 255)
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
}


