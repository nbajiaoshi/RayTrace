#ifndef HEAP_H
#define HEAP_H
#include "triangle.h"
#include <vector>
#include "SimpleObject.h"
using namespace std;
class Heap
{
public:
    Heap();
    void push(Edge* e);
    void shrink(Point** points,int& n);
    int size(){return edges.size();}
    Edge* operator [](int i){return edges[i];}
    Edge* pop();
protected:
    vector<Edge*> edges;
    void UpFilter(int i);
    void DownFilter(unsigned i);
    void swap(int i,int j);
    void remove(unsigned i);
    void upDate(unsigned i);
};

#endif // HEAP_H
