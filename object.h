//
// Created by J SL on 2023/6/17.
//

#ifndef RAYTRACE_OBJECT_H
#define RAYTRACE_OBJECT_H
#include "ray.h"
#include <vector>
#include <memory>
#include "AABB.h"
class Material;
struct hit_record{
    Vector3f point;
    Vector3f normal;
    float t;
    float u;
    float v;
    std::shared_ptr<Material> material;
    bool front_face;
    void setNormal(const Ray& ray,Vector3f const& n)
    {
        if (dot(ray.direction(),n) > 0){
            normal = -n;
            front_face = false;
        }
        else
        {
            normal = n;
            front_face = true;
        }
    }
};
class Object : public std::enable_shared_from_this<Object>{
public:
    virtual ~Object() = default;
    virtual bool hit(const Ray& ray,float t_min,float t_max,hit_record& record) const= 0;
    virtual AABB bounding_box() const = 0;
};
class hit_table : public Object{
public:
    hit_table(){}
    ~hit_table() {m_objects.clear();}
    hit_table(std::shared_ptr<Object> const& obj) { add(obj);}
    void clean() {m_objects.clear();}
    std::vector<std::shared_ptr<Object>> Objects() const {return m_objects;}
    void add(std::shared_ptr<Object> obj) {m_objects.push_back(obj);_box = AABB(_box,obj->bounding_box());}
    virtual AABB bounding_box() const override {return _box;}
    virtual bool hit(const Ray& ray,float t_min,float t_max,hit_record& record) const override{
        bool hitted = false;
        float close_so_far = t_max;
        hit_record r;
        for(auto const& obj : m_objects){
            if (obj->hit(ray,t_min,close_so_far,r))
            {
                record = r;
                close_so_far = record.t;
                hitted = true;
            }
        }
        return hitted;
    }
private:
    std::vector<std::shared_ptr<Object>> m_objects;
    AABB _box;
};
class Sphere : public  Object{
public:
    Sphere(Vector3f const& c,float radius,std::shared_ptr<Material> const& material): _center(c),_radius(radius),_material(material),_isMoving(false)
    {
        auto vr = Vector3f(_radius);
        _box = AABB(c - vr,c+vr);
    }
    Sphere(Vector3f const& c,Vector3f const& c2,float radius,std::shared_ptr<Material> const& material): _center(c),_radius(radius),_material(material),_isMoving(true){
        _dir = c2 - c;
        auto vr = Vector3f(radius);
        AABB box1(c - vr,c+vr);
        AABB box2(c2-vr,c2+vr);
        _box = AABB(box1,box2);
    }
    virtual AABB bounding_box() const override {return _box;}
    virtual bool hit(const Ray& ray,float t_min,float t_max,hit_record& record) const override{
        auto center = _isMoving ? _center + _dir * ray.time() : _center;
        Vector3f oc = ray.origin() - center;
        float a = dot(ray.direction(),ray.direction());
        float b = dot(oc,ray.direction());
        float c = dot(oc,oc) - _radius * _radius;
        float delta = b * b - a *c;
        if (delta < 0)
            return false;
        float t = (-b - std::sqrtf(delta)) / a;
        if (t < t_min || t > t_max)
        {
            t = (-b + std::sqrtf(delta)) / a;
            if ( t < t_min || t > t_max )
                return false;
            record.t = t;
            record.setNormal(ray,(ray(t) - _center).normalize());
            record.point = ray(t);
            record.material = _material;
        }
        else
        {
            record.t = t;
            record.setNormal(ray,(ray(t) - _center).normalize());
            record.point = ray(t);
            record.material = _material;
        }

        return true;
    }
private:
    Vector3f _center;
    float _radius;
    std::shared_ptr<Material> _material;
    Vector3f _dir;
    bool _isMoving;

    AABB _box;
};

#endif //RAYTRACE_OBJECT_H
