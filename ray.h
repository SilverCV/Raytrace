//
// Created by J SL on 2023/6/17.
//

#ifndef RAYTRACE_RAY_H
#define RAYTRACE_RAY_H
#include "vec.h"
class Ray{
public:
    Ray(Vector3f const& origin,Vector3f const& direction,float t): _origin(origin), _direction(direction),_time(t){}
    Ray() = default;
    Vector3f origin() const{
        return _origin;
    }
    Vector3f direction() const {
        return _direction;
    }
    float time() const {
        return _time;
    }
    Vector3f operator()(float t) const{
        return _origin + t * _direction;
    }
private:
    Vector3f _origin;
    Vector3f _direction;
    float _time;
};
#endif //RAYTRACE_RAY_H
