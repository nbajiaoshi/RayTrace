#include <iostream>
#include "SimpleObject.h"
#include "vec4f.h"
#include <queue>
#include <set>
#include "triangle.h"
#include "heap.h"
using namespace std;
Point** points;
Heap heap;
int n;
void init(char* f1){
    CSimpleObject obj;
    obj.LoadFromObj(f1);
    /*for (int i = 0;i < obj.VSize();i++)
        cout << obj.V(i)[0] << ' '<<obj.V(i)[1]  << ' '<<obj.V(i)[2]  << '\n';
    for (int i = 0;i < obj.FSize();i++)
        cout << obj.F(i,0) << ' '<<obj.F(i,1) << ' '<<obj.F(i,2) << '\n';*/
    n = obj.VSize();
    points = new(Point*[n]);
    for (int i = 0;i < n;i++){
        points[i] = new Point(obj.V(i));
        points[i]->id = i;
    }

    int m = obj.FSize();
    int v[3];
    Edge* e;
    Triangle* tri;
    vector<Edge*> edges;
    for (int i = 0;i < m;i++){
        for (int j = 0;j < 3;j++)
            v[j] = obj.F(i,j);
        for (int v1 = 0;v1 < 2;v1++)
            for (int v2 = v1 + 1;v2 < 3;v2++){
                e = new Edge(points[v[v1]],points[v[v2]]);
                if (points[v[v1]]->AddEdge(e) && points[v[v2]]->AddEdge(e)){
                    edges.push_back(e);
                }
            }
        tri = new Triangle(points[v[0]],points[v[1]],points[v[2]]);
        for (int j = 0; j < 3;j++)
            points[v[j]]->triangles.push_back(tri);
    }
    for (int i = 0;i < n;i++)
        points[i]->renewQ();
    cout << heap.size() << endl;
    for (unsigned i = 0; i < edges.size();i++){
        edges[i]->renewDeta();
        heap.push(edges[i]);
    }
    /*cout << heap.size() << endl;
    cout << "end\n";*/
}

int main(int argc,char** argv)
{
    char* f1 = argv[1];
    char* f2 = argv[2];

    double  simplifyrate = atof(argv[3]);
    init(f1);
    int simplifym = int((1 - simplifyrate) * n);
    for (int i = 0; i < simplifym;i++){
        heap.shrink(points,n);
    }
    CSimpleObject obj(points,n);
    obj.SaveToObj(f2);
    return 0;
}

