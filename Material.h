//
// Created by J SL on 2023/6/19.
//

#ifndef RAYTRACE_MATERIAL_H
#define RAYTRACE_MATERIAL_H

#include "object.h"
#include "Texture.h"
class Material{
public:
    virtual bool scatter(const Ray& ray_in,const hit_record& rec,Vector3f & attenuation,Ray& scatter) const = 0;
};

class Lambertian : public  Material{
public:
    Lambertian(Vector3f const& color) : _albedo(color),IsUseTexture(false){}
    Lambertian(const std::shared_ptr<Texture>& texture):IsUseTexture(true),_tex(texture){}
    virtual bool scatter(const Ray& ray_in,const hit_record& rec,Vector3f& attenuation,Ray& scatter) const override{
        auto scatterd = ray_in.direction() + random_unit_sphere();
        scatter = Ray(rec.point,scatterd.normalize(), ray_in.time());
        attenuation = IsUseTexture ?_tex->get_color(rec.point) :  _albedo;
        return true;
    }
private:
    Vector3f _albedo;
    bool IsUseTexture;
    std::shared_ptr<Texture> _tex;
};

class Metal : public  Material{
public:
    Metal(Vector3f const& color,float f) : _albedo(color),_fuzy(f < 1 ? f : 1){}
    virtual bool scatter(const Ray& ray_in,const hit_record& rec,Vector3f& attenuation,Ray& scatter) const override{
        auto scatterd = reflect(ray_in.direction(),rec.normal);
        scatter = Ray(rec.point,_fuzy *random_unit_sphere() + scatterd.normalize(),ray_in.time());
        attenuation = _albedo;
        return (dot(scatterd,rec.normal) > 0);
    }
private:
    Vector3f _albedo;
    float _fuzy;
};

class Dielectric : public Material{
public:
    Dielectric(float etai):_ir(etai){}

    virtual bool scatter(const Ray& ray_in,const hit_record& rec,Vector3f& attenuation,Ray& scatter) const override{

        attenuation = Vector3f(1.0f);
        float etai_over_etat = rec.front_face ?  (1.0f / _ir) : _ir;
        auto v = ray_in.direction().normalize();
        float cos_theta = fmin(-dot(v,rec.normal),1.0f);
        float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

        Vector3f direction;
        if (etai_over_etat * sin_theta > 1.0f || reflectance(cos_theta,etai_over_etat) > random_float())
        {
            direction = reflect(v,rec.normal);
        }
        else
        {
            direction = refract(v,rec.normal,etai_over_etat);
        }
        scatter = Ray(rec.point,direction,ray_in.time());
        return true;
    }

private:
    float _ir;

private:
    float reflectance(float cosine,float ref_idx) const {
        auto r0 = (1.0f - ref_idx) / (1.0 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1- r0) * powf((1.0f-cosine),5);
    }
};
#endif //RAYTRACE_MATERIAL_H
