#pragma once
#include "Vec3f.h"
#include <vector>
#include <triangle.h>
template <typename T, int N> class Array
{
public:
    enum {_len = N};
    typedef T t_Val;
public:
    T& operator[] (int i)
    {
        assert(i>=0&&i<N);
        return _p[i];
    }
    const T& operator[] (int i) const
    {
        assert(i>=0&&i<N);
        return _p[i];
    }

protected:
    T _p[N];
};

class CSimpleObject
{
public:
    CSimpleObject(void);
    ~CSimpleObject(void);
    CSimpleObject(Point** points,int n);
public:
    bool IsLoaded() { return m_pVertexList!=NULL;}
    Vec3f V(int i) {return m_pVertexList[i];}
    int VSize() {return m_nVertices;}
    int F(int i,int j) {return m_pTriangleList[i][j];}
    int FSize() {return m_nTriangles;}
    void Destroy();
    bool LoadFromObj(const char* fn);
    bool SaveToObj(const char* fn);
protected:
    bool Parse(FILE* fp);
    bool CheckParse(int nVertices,std::vector<Array<int,3> > & vecTriangles);

protected:

    int             m_nVertices;
    int             m_nTriangles;
    Vec3f*          m_pVertexList;
    Array<int,3>*   m_pTriangleList;
};
namespace SimpleOBJ
{


}
