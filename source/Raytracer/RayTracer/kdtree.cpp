#include "kdtree.h"
#include <algorithm>
#include <iostream>
using namespace std;
KdTree::KdTree()
{
    NodeNum = 0;
}
bool operator <(const SortUnit& a1,const SortUnit& a2){
    if (fabs(a1.data - a2.data) < EPS){
        if (!a1.minmaxequal){
            if (a1.ismin)
                return false;
            else
                return true;
        }
        if (!a2.minmaxequal){
            if (a2.ismin)
                return true;
            else
                return false;
        }
        if (a1.id != a2.id)
            return a1.id < a2.id;
        if (a1.ismin)
            return true;
        else
            return false;

    }
    else
        if (a1.data < a2.data)
            return true;
        else
            return false;
}
void KdTree::append(Triangle* t){
    tri.append(t);
}
void KdTree::BuildTree(){
    root = new TreeNode;
    for (int d = 0;d <= 2;d++){
        AABB[d][0] = INF;
        AABB[d][1] = -INF;
        for (int i = 0; i < tri.size();i++){
            if (AABB[d][0] > tri[i]->AABB[d][0])
                AABB[d][0] = tri[i]->AABB[d][0];
            if (AABB[d][1] < tri[i]->AABB[d][1])
                AABB[d][1] = tri[i]->AABB[d][1];
        }
        qDebug() << AABB[d][0] << AABB[d][1];
    }
    BuildSubTree(root,tri,0);
}
void KdTree::BuildSubTree(TreeNode* sub_root,QList<Triangle*>& trilist,int dir){
    if (trilist.size() < 3){
        sub_root->tri = trilist;
        sub_root->isleaf = true;
        return;
    }
    QList<SortUnit> AABBsort;
    sub_root->id = NodeNum++;
    int n = trilist.size();
    for (int i = 0;i < n;i++){
        AABBsort.append(SortUnit(trilist[i]->AABB[dir][0],i,true,(fabs(trilist[i]->AABB[dir][0] - trilist[i]->AABB[dir][1]) < EPS)));
        AABBsort.append(SortUnit(trilist[i]->AABB[dir][1],i,false,(fabs(trilist[i]->AABB[dir][0] - trilist[i]->AABB[dir][1]) < EPS)));
    }
    double sum = 0;
    for (int i = 0;i < n;i++)
        sum += trilist[i]->S;
    qSort(AABBsort.begin(),AABBsort.end());
    double bestcost = INF;
    double leftsum = 0;
    int leftnum = 0;
    double rightsum = sum;
    int rightnum = n;
    int boundary = n;
    for (int i = 1;i < 2 * n - 1;i++){
        if (AABBsort[i - 1].ismin){
            leftsum += trilist[AABBsort[i - 1].id]->S;
            leftnum++;
        }
        if (!AABBsort[i].ismin){
            rightsum -= trilist[AABBsort[i].id]->S;
            rightnum --;
        }
        if ((rightnum * rightsum + leftnum * leftsum) < bestcost){
            bestcost = rightnum * rightsum + leftnum * leftsum;
            boundary = i;
        }
    }
    //cout << boundary << ' ' << n << endl;
    sub_root->borderline = AABBsort[boundary].data;
    sub_root->dir = dir;
    //boundary = n;

    QList<Triangle*> left,right;
    for (int i = 0;i < boundary;i++)
        if (AABBsort[i].ismin)
            left.append(trilist[AABBsort[i].id]);
    for (int i = boundary + 1;i < 2 * n;i++)
        if (!AABBsort[i].ismin)
            right.append(trilist[AABBsort[i].id]);
    if (SHOWTREE)
            qDebug() << n <<sub_root->id << left.size() << right.size()<<sub_root->dir;
    if (n < 3 || double(left.size()) * double(left.size()) + double(right.size())* double(right.size()) >= double(n)*double(n)){
        sub_root->tri = trilist;
        sub_root->isleaf = true;
        return;
    }
    sub_root->isleaf = false;
    sub_root->leftson = new TreeNode;
    sub_root->rightson = new TreeNode;
    BuildSubTree(sub_root->leftson,left,(dir + 1) % 3);
    BuildSubTree(sub_root->rightson,right,(dir + 1) % 3);

}

bool KdTree::findIntersectionInSubTree(Line l,TreeNode* sub_root,double tmin,double tmax,Triangle*& IntersectionTri,Vec3f& IntersectPoint) const{
    if (sub_root->isleaf){
        double t,dis = tmax;
        bool haveIntersection = false;
        Vec3f n;
        for (int i = 0;i < sub_root->tri.size();i++){
            t = sub_root->tri[i]->getIntersection(l);
            if (t > tmin && t < dis + EPS){
                haveIntersection = true;
                dis = t;
                IntersectionTri = sub_root->tri[i];
                IntersectPoint = l.start_point + l.dir * t;
            }
        }
        return haveIntersection;
    }
    double Ori = l.start_point[sub_root->dir],Dir = l.dir[sub_root->dir];
    if (abs(Dir) < EPS){
        if (Ori < sub_root->borderline)
            return findIntersectionInSubTree(l,sub_root->leftson,tmin,tmax,IntersectionTri,IntersectPoint);
        else
            return findIntersectionInSubTree(l,sub_root->rightson,tmin,tmax,IntersectionTri,IntersectPoint);
    }
    double t = (sub_root->borderline - Ori) / Dir;
    if (t < tmin){
        if (Dir > 0)
            return findIntersectionInSubTree(l,sub_root->rightson,tmin,tmax,IntersectionTri,IntersectPoint);
        else
            return findIntersectionInSubTree(l,sub_root->leftson,tmin,tmax,IntersectionTri,IntersectPoint);
    }
    if (t > tmax){
        if (Dir < 0)
            return findIntersectionInSubTree(l,sub_root->rightson,tmin,tmax,IntersectionTri,IntersectPoint);
        else
            return findIntersectionInSubTree(l,sub_root->leftson,tmin,tmax,IntersectionTri,IntersectPoint);
    }
    if (Dir > 0){
        if (findIntersectionInSubTree(l,sub_root->leftson,tmin,t,IntersectionTri,IntersectPoint))
            return true;
        return findIntersectionInSubTree(l,sub_root->rightson,t,tmax,IntersectionTri,IntersectPoint);
    }
    else {
        if (findIntersectionInSubTree(l,sub_root->rightson,tmin,t,IntersectionTri,IntersectPoint))
            return true;
        return findIntersectionInSubTree(l,sub_root->leftson,t,tmax,IntersectionTri,IntersectPoint);
    }
}
bool KdTree::getIntersection(Line l,Vec3f& N,Vec3f& IntersectPoint,Vec3f& Color,Material& material,double len_limit) const{
    Triangle* IntersectTri = NULL;
    double tmin[3],tmax[3];
    for (int d = 0; d < 3;d++){
        if (fabs(l.dir[d]) > EPS)
            if (l.dir[d] > 0){
                tmin[d] = (AABB[d][0] - l.start_point[d]) / l.dir[d];
                tmax[d] = (AABB[d][1] - l.start_point[d]) / l.dir[d];
            }
            else {
                tmax[d] = (AABB[d][0] - l.start_point[d]) / l.dir[d];
                tmin[d] = (AABB[d][1] - l.start_point[d]) / l.dir[d];
            }
        else{
            if (l.start_point[d] > AABB[d][1] || l.start_point[d] < AABB[d][0])
                return false;
            tmin[d] = -INF;
            tmax[d] = INF;
        }
    }
    if (max(tmin[0],max(tmin[1],tmin[2])) > min(tmax[0],min(tmax[1],tmax[2])))
        return false;
    if (findIntersectionInSubTree(l,root,EPS,len_limit,IntersectTri,IntersectPoint)){
        Color = IntersectTri->getColor(IntersectPoint);
        material = IntersectTri->material;
        N = IntersectTri->getN(IntersectPoint);
        N.Normalize();
        if (N * l.dir < 0){
            N = - N;
            material.refract_n = 1.0 / material.refract_n;
        }
        return true;
    }
    else
        return false;
}

