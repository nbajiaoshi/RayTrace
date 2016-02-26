#include "triangle.h"


bool Edge::operator <(const Edge& e)const{
    return Deta < e.Deta;
}
void Point::renewQ(){
    Q = Matrix4f();
    for (unsigned i = 0; i < triangles.size();i++)
        Q += *triangles[i]->matrix;
}

void Edge::renewDeta(){
    Vec4f v((a->p + b->p) / 2);
    Deta = v * (a->Q + b->Q) * v;
}
bool Edge::operator ==(const Edge& e)const{
    return (e.a == a && e.b == b) || (e.b == a && e.a == b);
}
bool Triangle::operator ==(const Triangle& tri)const{
    int same = 0;
    for (int i = 0;i < 3;i++)
        if (_p[i] == tri.a || _p[i] == tri.b ||_p[i] == tri.c)
            same++;
    if (same >= 3)
        return true;
    else
        return false;
}
int Triangle::HasPoints(Point* v1,Point* v2){
    int has = 0;
    for (int i = 0; i < 3;i++)
        if (v1 == _p[i] || v2 == _p[i])
            has++;
    return has;
}

void Triangle::Change(Point* v1,Point* v2,Point* v){
    for (int i = 0; i < 3;i++)
        if (v1 == _p[i] || v2 == _p[i])
            _p[i] = v;
    N = (b->p - a->p) ^ (c->p - b->p);
    N.Normalize();
    matrix = new Matrix4f(N,a->p);


}
bool Point::AddEdge(Edge* e){
    for (unsigned i = 0; i < edges.size();i++)
        if (*(edges[i]) == *e)
            return false;
    edges.push_back(e);
    return true;
}
void Point::Change(Point* v1,Point* v2,Point* v){
    int m = edges.size() - 1;
    for (unsigned i = 0; i < edges.size();i++)
        if (edges[i]->a == v1 || edges[i]->a == v2 || edges[i]->b == v1 || edges[i]->b == v2){
            edges[i] = edges[edges.size() - 1];
            edges.pop_back();
            i--;
        }
    for (unsigned i = 0; i < triangles.size();i++){
        switch (triangles[i]->HasPoints(v1,v2)) {
        case 1:
           // Q -= *triangles[i]->matrix;
            triangles[i]->Change(v1,v2,v);
            for (unsigned j = 0;j < i;j++)
                if (*(triangles[i]) == *(triangles[j])){
                    triangles[i] = triangles[triangles.size() - 1];
                    triangles.pop_back();
                    i--;
                    continue;
                }
           // Q += *triangles[i]->matrix;
            break;
        case 2:
           // Q -= *triangles[i]->matrix;
            triangles[i] = triangles[triangles.size() - 1];
            triangles.pop_back();
            i--;
            break;
        default:
            for (unsigned j = 0;j < i;j++)
                if (*(triangles[i]) == *(triangles[j])){
                    //Q -= *triangles[i]->matrix;
                    triangles[i] = triangles[triangles.size() - 1];
                    triangles.pop_back();
                    i--;
                    continue;
                }
            break;
        }
    }
    renewQ();
}

