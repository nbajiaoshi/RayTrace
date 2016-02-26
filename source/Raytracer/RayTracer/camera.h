#ifndef CAMERA_H
#define CAMERA_H
#include "Vec3f.h"
#include "scene.h"
#include "geometryelement.h"

class Camera
{
public:
    Camera(Vec3f eye,Vec3f,Vec3f dx,Vec3f dy,double AR,int w,int h,Scene* s);
    Vec3f** TakePicture();
 protected:
    Vec3f _eye;
    Vec3f _UpperLeft;
    Vec3f** _color;
    int _w;
    int _h;
    Scene* _scene;
  private:
    Vec3f _dx;
    Vec3f _dy;
    double aperture_radius;

};

#endif // CAMERA_H
