#ifndef SCENE_H
#define SCENE_H
#include "Vec3f.h"
#include "SimpleObject.h"
#include <vector>
#include "geometryelement.h"
#include "SimpleObject.h"
#include "kdtree.h"
using namespace std;
typedef bool bools[LightNum + 1][LightNum + 1];
class Scene
{
public:
    Scene();
    Vec3f RayTracer(Line l,Vec3f& IntersectPoint,double strength = 1.0) const;
protected:
    bool getIntersection(Line l,Vec3f& N,Vec3f& IntersectPoint,Vec3f& Color,Material& material,double len_limit = INF) const;
    void AddModel(char* Fn,Vec3f center = Vec3f(0,0,0),double scale = 1);
    Vec3f Transformation(Vec3f p);
    double getBrightness(Vec3f UL,Vec3f UR,Vec3f DR,Vec3f DL,Vec3f& p) const;
    double getBrightness(int U,int D,int L,int R,Vec3f& p,bools& Done,bools& Reach) const;
    void LightInit();
    QList<Object*> _objects;
    vector<Vec3f> _lightsources;    
    CSimpleObject _objmodel;
    KdTree tri_tree;
    QList<KdTree*> models;
    Vec3f LightUL,LightUR,LightDR,LightDL;
    Vec3f _Light[LightNum + 1][LightNum + 1];
};

#endif // SCENE_H
