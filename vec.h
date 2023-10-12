//
// Created by J SL on 2023/6/16.
//

#ifndef RAYTRACE_VEC_H
#define RAYTRACE_VEC_H
#include <climits>
#include <cmath>
#include <ostream>
#include <stdlib.h>
inline float random_float(){
    return rand() / ((float)RAND_MAX + 1.0f);
}

inline float random_float(float min,float max){
    return min + (max - min) * random_float();
}
class Vector3f{
public:
    Vector3f(float x,float y,float z):_x(x),_y(y),_z(z){}
    explicit Vector3f(float x = 0.0f):_x(x),_y(x),_z(x){}

    float x() const {
        return _x;
    }
    float y() const{
        return _y;
    }
    float z() const{
        return _z;
    }
    float& operator[](size_t idx){
        return (&_x)[idx];
    }
    const float& operator[](size_t idx) const{
        return (&_x)[idx];
    }
    Vector3f operator*(float t) const{
        return Vector3f(t * _x,t * _y,t * _z);
    }
    Vector3f operator/(float t) const {
        return Vector3f(_x / t,_y /t,_z /t);
    }
    Vector3f operator+(float t) const {
        return Vector3f(_x + t,_y + t,_z +t);
    }
    Vector3f operator-(float t) const{
        return Vector3f(_x - t,_y -t,_z -t);
    }
    Vector3f operator+(Vector3f const& v) const{
        return Vector3f(_x + v._x,_y + v._y,_z + v._z);
    }
    Vector3f operator-(Vector3f const& v) const{
        return Vector3f(_x - v._x,_y - v._y,_z - v._z);
    }
    Vector3f operator*(Vector3f const& v) const{
        return Vector3f(_x * v._x,_y * v._y,_z * v._z);
    }
    Vector3f operator/(Vector3f const& v) const{
        return Vector3f(_x / v._x,_y / v._y,_z / v._z);
    }
    Vector3f operator-() const{
        return Vector3f(-_x,-_y,-_z);
    }
    Vector3f& operator+=(float x){
        *this += Vector3f(x);
        return *this;
    }
    Vector3f& operator-=(float x){
        *this -= Vector3f(x);
        return *this;
    }
    Vector3f& operator*=(float r){
        *this *= Vector3f(r);
        return *this;
    }
    Vector3f& operator/=(float r){
        *this /= Vector3f(r);
        return *this;
    }
    Vector3f& operator*=(Vector3f const& v){
        _x *= v._x;
        _y *= v._y;
        _z *= v._z;
        return *this;
    }
    Vector3f& operator/=(Vector3f const& v){
        _x /= v._x;
        _y /= v._y;
        _z /= v._z;
        return *this;
    }
    Vector3f& operator+=(Vector3f const& v1){
        _x += v1._x;
        _y += v1._y;
        _z += v1._z;
        return *this;
    }
    Vector3f& operator-=(Vector3f const& v){
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }
    friend  Vector3f operator*(float t,const Vector3f& v){
        return v * t;
    }

    float norm() const {
        return std::sqrt(_x * _x + _y * _y + _z * _z);
    }
    Vector3f normalize() const {
        return *this / std::sqrt(_x * _x + _y * _y + _z * _z);
    }

    friend std::ostream& operator<<(std::ostream &os,Vector3f const& v){
        os << "[" << v._x << "," << v._y << "," << v._z << "]" << std::endl;
        return os;
    }
    inline static Vector3f random(){
        return Vector3f(random_float(),random_float(),random_float());
    }
    inline static  Vector3f random(float min,float max){
        return Vector3f(random_float(min,max),random_float(min,max),random_float(min,max));
    }
private:
    float _x;
    float _y;
    float _z;
};
inline float dot(Vector3f const& v,Vector3f const& v2){
    return v[0]*v2[0] + v[1] * v2[1] + v[2]*v2[2];
}

inline Vector3f cross(Vector3f const& v1,const Vector3f& v2){
    return Vector3f(v1[1] * v2[2] - v1[2] * v2[1],v1[2] * v2[0] - v1[0] * v2[2],v1[0] * v2[1] - v1[1] * v2[0]);
}
inline Vector3f reflect(const Vector3f& v,Vector3f const& n)
{
    return v - 2.0f* dot(v,n) * n;
}
inline Vector3f refract(Vector3f const& v,const Vector3f& n,float etai_over_etat){
    auto cos_theta = -dot(v,n);
    auto R_prep = etai_over_etat * (v + cos_theta * n);
    auto R_parallel = -sqrtf(fabsf(1.0f - R_prep.norm() * R_prep.norm())) * n;
    return R_prep + R_parallel;
}
inline float radians(float degree) {
    return M_PI * degree / 180.0f;
}
inline float degree(float radian){
    return radian / M_PI * 180.0f;
}
inline Vector3f random_sphere(){
    while(true) {
        Vector3f p = Vector3f::random(-1, 1);
        if (p.norm() >= 1) continue;
        return p;
    }
}
inline Vector3f random_unit_sphere(){
    return random_sphere().normalize();
}
#endif //RAYTRACE_VEC_H
