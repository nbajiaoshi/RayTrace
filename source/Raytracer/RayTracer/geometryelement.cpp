#include "geometryelement.h"
#include "qdebug.h"
#include "math.h"
void Object::LoadTexture(QString filename,double amplification){
    pixel_amplification_rate = amplification;
    use_texture = true;
    _texture = new QImage(filename);
}
Vec3f Ball::getColor(Vec3f p){
    if (!use_texture)
        return material.color;
    p = p -center;
    double u,v;
    if (fabs(p.x) < EPS && fabs(p.y) < EPS)
        u = 0;
    else if (fabs(p.x) < EPS)
        u = p.y > 0?1:-1;
    else
        u = atan(p.y / p.x) / Pi * 2;
    v = asin(p.z / radius) / Pi * 2;
    u = (u + 1) / 2;
    v = (v + 1) / 2;
    int x = int(u * _texture->width());
    if (x >= _texture->width()) x--;
    int y = int(v * _texture->height());
    if (y >= _texture->height()) y--;
    if (y < -210000000)
        qDebug() << p.z << v;
    QRgb c = _texture->pixel(x,y);
    return Vec3f((c << 8) >> 24,(c << 16) >> 24,(c << 24) >> 24);
}
double Ball::getIntersection(Line line){
    Vec3f l = center - line.start_point;
    double Tp = l * line.dir;
    double d2 = l.L2Norm_Sqr() - Tp * Tp;
    if (sqrt(d2) >= radius)
        return 0;
    double t1 = sqrt(radius * radius - d2);
    if (l.Len() > radius + EPS)
        return Tp - t1;
    else
        return Tp + t1;
}
Vec3f Ball::getN(Vec3f IntersectPoint){
    return IntersectPoint - center;
}
Vec3f Plane::getColor(Vec3f p){
    if (!use_texture)
        return material.color;
    int x = abs(abs(int(p.x * pixel_amplification_rate) + _texture->width() / 2) % (_texture->width() * 2) - _texture->width()) % _texture->width();
    int y = abs(abs(int(p.y * pixel_amplification_rate) + _texture->height() / 2) % (_texture->height() * 2) - _texture->height()) % _texture->height();
    QRgb c = _texture->pixel(x,y);
    return Vec3f((c << 8) >> 24,(c << 16) >> 24,(c << 24) >> 24) * 1.1;

}

double Plane::getIntersection(Line line){
    if (fabs(line.dir * N) < EPS){
        return -1;
    }
    else
        return -(D + N * line.start_point) / (line.dir * N);
}
Vec3f Plane::getN(Vec3f){
    return N;
}


void Material::setGreenBall(){
    color = Vec3f(0,255,0);
    Ka = 0.1;
    Kd = 0.5;
    Ks = 0.4;
    Ksn = 10;
    reflect_rate = 0.4;
    refract_rate = 0;
    refract_n = 1.55;
    diffuse_reflectance = 1 - reflect_rate - refract_rate;
}

void Material::setRedBall(){
    color = Vec3f(255,255,255);
    Ka = 0.2;
    Kd = 0;
    Ks = 0.8;
    Ksn = 6;
    reflect_rate = 0;
    refract_rate = 0.95;
    refract_n = 1.7;
    absorptivity = 0.5;
    diffuse_reflectance = 0.2;
}
void Material::setLight(){
    color = Vec3f(255,255,255);
    Ka = 1;
    Kd = 0;
    Ks = 0;
    Ksn = 0;
    reflect_rate = 0;
    refract_rate = 0;
    refract_n = 1;
    diffuse_reflectance = 1 - reflect_rate - refract_rate;
}
void Material::setStone(){
    reflect_rate = 0;
    Ka = 0.35;
    Kd = 0.5;
    Ks = 0.15;
    Ksn = 6;
    diffuse_reflectance = 1 - reflect_rate - refract_rate;
}
void Material::setSmoothSurface(){
    reflect_rate = 0.3;
    Ka = 0.35;
    Kd = 0.6;
    Ks = 0.05;
    Ksn = 6;
    diffuse_reflectance = 1 - reflect_rate - refract_rate + 0.05;
}
void Material::setNormalPlane(){
    color = Vec3f(0,150,254);
    Ka = 0.35;
    Kd = 0.45;
    Ks = 0.2;
    Ksn = 15;
    reflect_rate = 0.3;
    refract_rate = 0;
    refract_n = 1;
    diffuse_reflectance = 1 - reflect_rate - refract_rate;
}
void Material::setWall(){
    color = Vec3f(220,220,220);
    Ka = 0.35;
    Kd = 0.45;
    Ks = 0.2;
    Ksn = 15;
    reflect_rate = 0;
    refract_rate = 0;
    refract_n = 1;
    diffuse_reflectance = 1 - reflect_rate - refract_rate;
}
void Material::setCube(){
    color = Vec3f(255,255,0);
    Ka = 0.05;
    Kd = 0.65;
    Ks = 0.35;
    Ksn = 3;
    reflect_rate = 0.1;
    refract_rate = 0;
    refract_n = 1.2;
    diffuse_reflectance = 1 - reflect_rate - refract_rate + 0.1;
}

Ball::Ball(Vec3f c,double r,Vec3f color):center(c),radius(r){
    material.setGreenBall();
    material.color = color;
}
Plane::Plane(Vec3f p,Vec3f n):N(n){
    N.Normalize();
    D = - n * p;
    material.setNormalPlane();
}
Plane::Plane(double d,Vec3f n):D(d),N(n){
    N.Normalize();
    material.setNormalPlane();
}
double Triangle::Det3(Vec3f a, Vec3f b, Vec3f c){
    return a.x * (b.y * c.z - b.z * c.y)
         + a.y * (b.z * c.x - b.x * c.z)
         + a.z * (b.x * c.y - b.y * c.x);
}

Triangle::Triangle(Vec3f p1, Vec3f p2, Vec3f p3):
    P({p1,p2,p3}),N((p2 - p1) ^ (p3 - p2)),E1(P[0] - P[1]),E2(P[0] - P[2])
{
    N.Normalize();
    material.setCube();
    for (int i = 0; i <= 2;i++)
        for (int j = 0;j <= 1;j++)
            AABB[i][j] = p1[i];
    for (int i = 1;i <3;i++)
        for (int j = 0;j <3;j++){
            if (P[i][j] < AABB[j][0])
                AABB[j][0] = P[i][j];
            if (P[i][j] > AABB[j][1])
                AABB[j][1] = P[i][j];
        }
    double h = (p2 - p1) * (p3 - p2);
    S = (p3 - p2).Len() * sqrt((p2 - p1).L2Norm_Sqr() - h*h) / 2;
}
Vec3f Triangle::getColor(Vec3f){
    return material.color;
}
double Triangle::getIntersection(Line l){
    Vec3f S = P[0] - l.start_point;
    double d = Det3(l.dir,E1,E2);
    double b = Det3(l.dir,S,E2) / d;
    if (b < -EPS || b > 1)
        return -1;
    double y = Det3(l.dir,E1,S) / d;
    if (b > -EPS && y > -EPS && b + y < 1 + EPS)
        return Det3(S,E1,E2) / d;
    else
        return -1;

}
Vec3f Triangle::getN(Vec3f){
    return N;
}
