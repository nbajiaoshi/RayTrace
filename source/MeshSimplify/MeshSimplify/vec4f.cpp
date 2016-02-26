#include "vec4f.h"

double Vec4f::operator *(const Vec4f& v) const{
    return a*v.a + b * v.b + c * v.c + d * v.d;
}
Vec4f Vec4f::operator *(const Matrix4f& m) const{
    Vec4f v(0,0,0,0);
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            v._p[i] += (_p[j] * m._m[i][j]);
    return v;
}

Matrix4f::Matrix4f(const Vec3f& N,const Vec3f& p){
    double _p[4] = {N.x,N.y,N.z,- N * p};
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            _m[i][j] = _p[i] * _p[j];
}

Vec4f Matrix4f::operator *(const Vec4f& p) const{
    Vec4f v(0,0,0,0);
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            v._p[i] += (p._p[j] * _m[j][i]);
    return v;
}
void Matrix4f::operator +=(const Matrix4f& m){
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            _m[i][j] += m._m[i][j];
}
void Matrix4f::operator -=(const Matrix4f& m){
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            _m[i][j] -= m._m[i][j];

}
Matrix4f Matrix4f::operator +(const Matrix4f& m) const{
    Matrix4f matrix;
    for (int i = 0; i < 4;i++)
        for (int j = 0; j < 4;j++)
            matrix._m[i][j] = m._m[i][j] + _m[i][j];
    return matrix;
}
