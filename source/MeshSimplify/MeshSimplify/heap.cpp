#include "heap.h"
#include <stdio.h>
ostream& operator <<(ostream& o,Vec3f& v){o << v.x << ' '<<v.y << ' ' << v.z << endl; return o;}
Heap::Heap()
{
}
void Heap::push(Edge* e){
    edges.push_back(e);
    e->id = edges.size() - 1;
    UpFilter(edges.size() - 1);
}

void Heap::shrink(Point** points,int& n){
    Edge* e = pop();
    Point* p = new Point((e->a->p + e->b->p) / 2);
    points[e->b->id] = points[n - 1];
    points[e->b->id]->id = e->b->id;
    points[e->a->id] = p;
    points[e->a->id]->id = e->a->id;
    n--;
    Point* q;
    Edge* edge;
    p->triangles = e->a->triangles;
    for (unsigned i = 0; i < e->b->triangles.size();i++)
        p->triangles.push_back(e->b->triangles[i]);
    p->Change(e->a,e->b,p);
    for (unsigned i = 0;i < e->a->edges.size();i++){
        q = e->a->edges[i]->Other(e->a);
        remove(e->a->edges[i]->id);
        if (q != e->b){
            edge = new Edge(p,q);
            if (edge->a->AddEdge(edge) && edge->b->AddEdge(edge))
                push(edge);
        }
    }
    for (unsigned i = 0;i < e->b->edges.size();i++){
        q = e->b->edges[i]->Other(e->b);
        if (q != e->a){
            remove(e->b->edges[i]->id);
            edge = new Edge(p,q);
            if (edge->a->AddEdge(edge) && edge->b->AddEdge(edge))
                push(edge);
        }
    }
    for (unsigned i = 0;i<p->edges.size();i++)
        p->edges[i]->Other(p)->Change(e->a,e->b,p);
    for (unsigned i = 0;i<p->edges.size();i++){
        q = p->edges[i]->Other(p);

        for (unsigned j = 0; j < q->edges.size();j++){
            q->edges[j]->renewDeta();
            upDate(q->edges[j]->id);
        }

    }
}

Edge* Heap::pop(){
    Edge* e = edges[0];
    swap(0,edges.size() - 1);
    edges.pop_back();
    DownFilter(0);
    return e;
}

void Heap::UpFilter(int i){
    while (i > 0 && *edges[i] < *edges[i/2]){
        swap(i,i/2);
        i /= 2;
    }

}

void Heap::DownFilter(unsigned i){
    while (i * 2 < edges.size() && (*edges[i * 2] < *edges[i] || (i * 2 + 1 < edges.size() && *edges[i * 2 + 1] < *edges[i]))){
        if (*edges[i * 2] < *edges[i] && i * 2 + 1 < edges.size() && *edges[i * 2 + 1] < *edges[i]){
            if (*edges[i * 2] < *edges[i * 2 + 1]){
                swap(i,i * 2);
                i = i * 2;
            }
            else{
                swap(i,i * 2 + 1);
                i = i * 2 + 1;

            }
        }
        else{
            if (*edges[i * 2] < *edges[i]){
                swap(i,i * 2);
                i = i * 2;
            }
            else{
                swap(i,i * 2 + 1);
                i = i * 2 + 1;

            }
        }
    }
}

void Heap::swap(int i,int j){
    Edge* t = edges[i];
    edges[i] = edges[j];
    edges[j] = t;
    edges[i]->id = i;
    edges[j]->id = j;
}
void Heap::remove(unsigned i){
    if (i < 0 || i >= edges.size())
        return;
    edges[i]->id = -1;
    if (i == edges.size() - 1){
        edges.pop_back();
        return;
    }
    swap(i,edges.size() - 1);
    edges.pop_back();
    DownFilter(i);
}

void Heap::upDate(unsigned i){
    UpFilter(i);
    DownFilter(i);
}
