#include "scene.h"
#include <math.h>
#include "qdebug.h"
double Len(Vec3f l){
    return sqrt(l.L2Norm_Sqr());
}
void debug(Vec3f v){
    qDebug() << v.x << ' ' << v.y << ' ' << v.z;
}

Scene::Scene()
{
    Object* object;
    object = new Ball(Vec3f(1,0,-0.71),0.3);
    _objects.push_back(new Ball(Vec3f(-5,0,0.5),1.5,Vec3f(210,210,210)));
    _objects.push_back(new TextureBall(Vec3f(0,1.6,-0.2),0.8,":/texture/stone.jpg"));
    _objects.push_back(new Ball(Vec3f(1.1,0.6,-0.5),0.5,Vec3f(100,100,255)));
    _objects.push_back(new GlassBall(Vec3f(1.2,-0.2,-0.8),0.2));
    _objects.push_back(new GlassBall(Vec3f(1.9,0.5,-0.85),0.15));
    _objects.push_back(new GlassBall(Vec3f(1.7,0.1,-0.85),0.15));
    _objects.push_back(new GlassBall(Vec3f(1.3,-0.8,-0.85),0.15));
    //_objects.push_back(new GlassBall(Vec3f(0,0,0),1));
    object = new Plane(Vec3f(0,0,-1),Vec3f(0,0,1));
    object->LoadTexture(":/texture/floor.bmp",100);
    object->material.setSmoothSurface();
    _objects.push_back(object);
    object = new Plane(Vec3f(0,0,30),Vec3f(0,0,-1));
    object->LoadTexture(":/texture/sky.jpg",10);
    object->material.setStone();
    _objects.push_back(object);
    _objects.push_back(new Plane(Vec3f(-10,0,0),Vec3f(-1,1,0)));
    _objects.last()->material.setWall();
    _objects.push_back(new Plane(Vec3f(-10,0,0),Vec3f(-1,-1,0)));
    _objects.last()->material.setWall();
    AddModel("G:/lesson/CSGraphic/Raytracer/resource/fixed.perfect.dragon.100K.0.07.obj",Vec3f(0.7,-1.5,-0.3),1);
    AddModel("G:/lesson/CSGraphic/Raytracer/resource/bunny.fine.obj",Vec3f(-0.8,-3.5,-1.335),10);
    //AddModel("G:/lesson/CSGraphic/Raytracer/resource/sphere.obj",Vec3f(0,0,0),1);
    LightInit();
    qDebug() << "finish";
}
void Scene::LightInit(){
    Vec3f light(1,-1,4);
    LightUL = light + Vec3f(-Nx * dxy,-Ny * dxy,0);
    LightUR = light + Vec3f(Nx * dxy,-Ny * dxy,0);
    LightDL = light + Vec3f(-Nx * dxy,Ny * dxy,0);
    LightDR = light + Vec3f(Nx * dxy,Ny * dxy,0);
    for (int i = 0; i <= LightNum;i++)
        for (int j = 0; j <= LightNum;j++)
            _Light[i][j] = LightUL + (LightDL - LightUL) * (1.0*i/LightNum) + (LightUR - LightUL) * (1.0*j/LightNum);
    Object* object;
    object = new Triangle(LightDR,LightDL,LightUR);
    object->material.setLight();
    _objects.push_back(object);
    object = new Triangle(LightUL,LightDL,LightUR);
    object->material.setLight();
    _objects.push_back(object);
    /*if (softshape)
        for (int x = -Nx; x <= Nx;x++)
            for (int y = -Ny; y <= Ny;y++)
                _lightsources.push_back(Vec3f(x * dxy,y * dxy,0)+ light);
    else*/
    _lightsources.push_back(light);
}

Vec3f Scene::Transformation(Vec3f p){
    return Vec3f(p.z,p.x,p.y);
}

void Scene::AddModel(char* Fn,Vec3f center,double scale){
    _objmodel.LoadFromObj(Fn);
    KdTree* model = new KdTree;
    for (int i = 0;i <_objmodel.FSize();i++){
        model->append(new Triangle(
            Transformation(_objmodel.F(i,0)) * scale + center,
            Transformation(_objmodel.F(i,1))* scale + center,
            Transformation(_objmodel.F(i,2)) * scale + center));        
    }
    if (models.size())
        model->setRedBall();
    model->BuildTree();
    models.append(model);
}

bool Scene::getIntersection(Line l,Vec3f& N,Vec3f& IntersectPoint,Vec3f& Color,Material& material,double len_limit) const{
    double t,dis = len_limit;
    bool haveIntersection = false;
    Vec3f n;
    for (int i = 0;i < models.size();i++)
        if (models[i]->getIntersection(l,N,IntersectPoint,Color,material,dis)){
            haveIntersection = true;
            dis = (IntersectPoint - l.start_point).Len();
        }
    /*if (tri_tree.getIntersection(l,N,IntersectPoint,Color,material,len_limit)){
        haveIntersection = true;
        dis = (IntersectPoint - l.start_point).Len();
    }*/
    for (int i = 0;i < _objects.size();i++){
        t = _objects[i]->getIntersection(l);
        if (t > EPS && t < dis){
            haveIntersection = true;
            dis = t;
            IntersectPoint = l.start_point + l.dir * t;
            N = _objects[i]->getN(IntersectPoint);
            material = _objects[i]->material;
            Color =_objects[i]->getColor(IntersectPoint);
        }
    }

    N.Normalize();
    if (N * l.dir < 0){
        N = - N;
        material.refract_n = 1.0 / material.refract_n;
    }
    return haveIntersection;
}
double Scene::getBrightness(int U,int D,int L,int R,Vec3f& p,bools& Done,bools& Reach) const{
   // qDebug() << U << D << L << R;
    Vec3f a,b,c;
    Material d;
    Vec3f UL = _Light[U][L] - p;
    Vec3f UR = _Light[U][R] - p;
    Vec3f DL = _Light[D][L] - p;
    Vec3f DR = _Light[D][R] - p;
    bool ul = Done[U][L]?Reach[U][L]:getIntersection(Line(p,UL),a,b,c,d,Len(UL)- EPS);
    if (!Done[U][L]){
        Reach[U][L] = ul;
        Done[U][L] = true;
    }
    bool ur = Done[U][R]?Reach[U][R]:getIntersection(Line(p,UR),a,b,c,d,Len(UR)- EPS);
    if (!Done[U][R]){
        Reach[U][R] = ur;
        Done[U][R] = true;
    }
    bool dl = Done[D][L]?Reach[D][L]:getIntersection(Line(p,DL),a,b,c,d,Len(DL)- EPS);
    if (!Done[D][L]){
        Reach[D][L] = dl;
        Done[D][L] = true;
    }
    bool dr = Done[D][R]?Reach[D][R]:getIntersection(Line(p,DR),a,b,c,d,Len(DR)- EPS);
    if (!Done[D][R]){
        Reach[D][R] = dr;
        Done[D][R] = true;
    }
    if (ul && ur && dr && dl)
        return 0;
    if (!(ul || ur || dr || dl))
        return 1;
    if (D <= U + 1)
        return 0.5;
    return 0.25*(
                getBrightness(U,(U + D)/2,L,(L + R)/2,p,Done,Reach) +
                getBrightness((U + D)/2,D,(L + R)/2,R,p,Done,Reach) +
                getBrightness(U,(U + D)/2,(L + R)/2,R,p,Done,Reach) +
                getBrightness((U + D)/2,D,L,(L + R)/2,p,Done,Reach)
                );
}
double Scene::getBrightness(Vec3f UL,Vec3f UR,Vec3f DR,Vec3f DL,Vec3f& p) const{
    Vec3f a,b,c;
    Material d;
    bool ul = getIntersection(Line(p,UL - p),a,b,c,d,Len(UL - p)- EPS);
    bool ur = getIntersection(Line(p,UR - p),a,b,c,d,Len(UR - p)- EPS);
    bool dr = getIntersection(Line(p,DR - p),a,b,c,d,Len(DR - p)- EPS);
    bool dl = getIntersection(Line(p,DL - p),a,b,c,d,Len(DL - p)- EPS);
    if (ul && ur && dr && dl)
        return 0;
    if (!(ul || ur || dr || dl))
        return 1;
    if ((UL - DR).Len() < dxy * 2.5)
        return 0.5;
    Vec3f M = (UL + DR) * 0.5;
    Vec3f U = (UL + UR) * 0.5;
    Vec3f D = (DL + DR) * 0.5;
    Vec3f R = (DR + UR) * 0.5;
    Vec3f L = (UL + DL) * 0.5;
    return 0.25 * (
                getBrightness(UL,U,M,L,p) +
                getBrightness(U,UR,R,M,p) +
                getBrightness(M,R,DR,D,p) +
                getBrightness(L,M,D,DL,p)
                );
}

Vec3f Scene::RayTracer(Line l,Vec3f& IntersectPoint,double strength) const{
    if (strength < 0.01)
        return Vec3f(0,0,0);
    Vec3f N;
    Material material;
    Vec3f Color;
    if (getIntersection(l,N,IntersectPoint,Color,material)){
        double len = (IntersectPoint - l.start_point).Len();
        if ((l.start_point + l.dir * len - IntersectPoint).Len() > EPS){
            qDebug() << "fuck" << (l.start_point + l.dir * len - IntersectPoint).Len();
            l.start_point.debug();
            l.dir.debug();
            IntersectPoint.debug();
        }
        if (material.refract_n > 1)
            strength *= exp(0.05 -len * material.absorptivity);
        double I = 0;
        /*for (unsigned i = 0; i < _lightsources.size();i++){
            Line light(IntersectPoint,_lightsources[i] - IntersectPoint);
            if (!getIntersection(light,a,b,c,d,Len(_lightsources[i] - IntersectPoint)- EPS)){
                Vec3f Ref =  light.dir - N * 2 * (light.dir * N);
                double Ii = material.Kd * abs(N * light.dir);
                if (Ref * l.dir > EPS)
                    Ii += material.Ks * pow(Ref * l.dir,material.Ksn);
                I += Ii * Ii/_lightsources.size();
            }
        }*/
        Line light(IntersectPoint,_lightsources[0] - IntersectPoint);
        Vec3f Ref =  light.dir - N * 2 * (light.dir * N);
        double Ii = material.Kd * abs(N * light.dir);
        if (Ref * l.dir > EPS)
            Ii += material.Ks * pow(Ref * l.dir,material.Ksn);
        double Brightness;
        if (softshape){
            bools Done,Reach;
            memset(Done,0,sizeof(Done));
            //Brightness = getBrightness(LightUL,LightUR,LightDR,LightDL,IntersectPoint);
            Brightness = getBrightness(0,LightNum,0,LightNum,IntersectPoint,Done,Reach);
        }
        else{
            Vec3f a,b,c;
            Material d;
            Brightness = getIntersection(light,a,b,c,d,Len(_lightsources[0] - IntersectPoint)- EPS)?0:1;
        }
        I = Ii * Brightness + material.Ka;
        if (I > 1 - EPS){
            I = 1 - EPS;
        }
        I = I * strength;
        double Cosi = l.dir * N;
        double Sini = abs(Cosi) < 1 - EPS?sqrt(1 - Cosi * Cosi):0;
        double Sinr = Sini * material.refract_n;
        Line reflect(IntersectPoint,l.dir - N * 2 * Cosi);
        Line refract = reflect;
        if (Sinr < 1 - EPS){
            double Cosr = sqrt(1 - Sinr * Sinr);
            Vec3f tmp = l.dir - N * Cosi;
            tmp.Normalize();
            Vec3f rf =  N * Cosr + tmp * Sinr;
            refract = Line(IntersectPoint,rf);
        }
        if (REF){
            Vec3f iii;
            Color = Color * I * material.diffuse_reflectance
                    + Color.mul(RayTracer(reflect,iii,material.reflect_rate * strength))
                    + Color.mul(RayTracer(refract,iii,material.refract_rate * strength));
        }
        else
            Color = Color * I * material.diffuse_reflectance;
        Color.Fix255();
        return Color;
    }
    else{
        double I = 1;
        for (unsigned i = 0; i < _lightsources.size();i++){

        }
        return Vec3f(0,0,0) * I * strength;
    }
}

