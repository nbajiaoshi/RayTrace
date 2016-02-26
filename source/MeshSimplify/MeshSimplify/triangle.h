#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <vector>
#include "Vec3f.h"
#include "vec4f.h"
using namespace std;
struct Edge;
struct Triangle;
struct Point{
    Point(Vec3f pp):p(pp){}
    Vec3f p;
    vector<Edge*> edges;
    vector<Triangle*> triangles;
    Matrix4f Q;
    int id;
    void renewQ();
    bool AddEdge(Edge* e);
    void Change(Point* v1,Point* v2,Point* v);
};
struct Edge{
    Edge(Point* aa,Point* bb):a(aa),b(bb){}
    Point* a,* b;
    bool operator <(const Edge& e)const;
    bool operator ==(const Edge& e)const;
    Point* Other(Point* p){if (p == a) return b;else return a;}
    double Deta;
    int id;
    void renewDeta();
};

struct Triangle
{
public:
    Triangle(Point* aa,Point* bb,Point* cc):a(aa),b(bb),c(cc),N((b->p - a->p) ^ (c->p - b->p)){
        N.Normalize();
        matrix = new Matrix4f(N,aa->p);
    }
    bool operator ==(const Triangle& e)const;
    union
    {
        struct
        { Point* _p[3]; };
        struct
        { Point* a,* b,* c; };
    };
    int HasPoints(Point* v1,Point* v2);
    void Change(Point* v1,Point* v2,Point* v);
    Vec3f N;
    Matrix4f* matrix;
};

#endif // TRIANGLE_H
