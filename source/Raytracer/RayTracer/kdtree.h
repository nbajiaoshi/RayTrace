#ifndef KDTREE_H
#define KDTREE_H
#include "geometryelement.h"
#include <QList>
using namespace std;
/*enum Direction{
    X = 0,Y = 1,Z = 2
};*/
struct TreeNode{
public:
    TreeNode():dir(0),leftson(NULL),rightson(NULL){}
public:
    QList<Triangle*> tri;
    double borderline;
    int dir;
    bool isleaf;
    TreeNode*leftson,*rightson;
    int id;
};
struct SortUnit{
    SortUnit(){}
    SortUnit(double d,int i,bool mi,bool equal):data(d),id(i),ismin(mi),minmaxequal(equal){}
    double data;
    int id;
    bool ismin;
    bool minmaxequal;
};
bool operator <(const SortUnit& a1,const SortUnit& a2);
class KdTree
{
public:
    KdTree();
    bool getIntersection(Line l,Vec3f& N,Vec3f& IntersectPoint,Vec3f& Color,Material& material,double len_limit = INF) const;
    void BuildTree();
    void append(Triangle* t);
    void setRedBall(){for (int i = 0;i < tri.size();i++) tri[i]->material.setRedBall();}
protected:
    TreeNode* root;
    QList<Triangle*> tri;
    void BuildSubTree(TreeNode* sub_root,QList<Triangle*>& trilist,int dir);
    bool findIntersectionInSubTree(Line l,TreeNode* sub_root,double tmin,double tmax,Triangle*& IntersectionTri,Vec3f& IntersectPoint) const;
    double AABB[3][2];
private:
    int NodeNum;
    double max(const double a,const double b) const{if (a>b) return a; else return b;}
    double min(const double a,const double b) const{if (a<b) return a; else return b;}
};


#endif // KDTREE_H
