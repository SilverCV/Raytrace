//
// Created by J SL on 2023/6/17.
//

#ifndef RAYTRACE_CAMERA_H
#define RAYTRACE_CAMERA_H
#include "ray.h"

class Camera{
public:
    Camera(Vector3f const& origin = Vector3f(0.0f,0.0f,0.0f),Vector3f const& center = Vector3f(0,0,0),Vector3f up=Vector3f(0,1.0,0),float fovy = 90.0f,int w = 0,int h=0):
    _origin(origin),_fovy(fovy),_width(w),_height(h)
    {
        _dis = (origin - center).norm();
        _aspectio = static_cast<float>(w )/ h;
        _front = (origin - center).normalize();
        _right = cross(_front,up).normalize();
        _up = cross(_right,_front).normalize();
    }

    Ray get_ray(float i,float j) const {
        float t = std::tanf(radians(_fovy / 2.0f)) ;
        auto x = (2.0f *(i+ 0.5f) / static_cast<float>(_width) - 1.0f) * t * _aspectio ;
        auto y = (1.0f - 2.0f *(j + 0.5f) / static_cast<float>(_height)) * t ;
        auto z = 1;
        auto dir = x * _right + y * _up - z * _front;
        auto time = random_float();
        return Ray(_origin,dir.normalize(),time);
    }

private:
    Vector3f _up;
    Vector3f _front;
    Vector3f _right;
    Vector3f _origin;
    float _fovy;
    float _aspectio;
    int _width;
    int _height;
    float _dis;
};
#endif //RAYTRACE_CAMERA_H
