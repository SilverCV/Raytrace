//
// Created by J SL on 2023/9/24.
//

#ifndef RAYTRACE_AABB_H
#define RAYTRACE_AABB_H
#include "vec.h"
#include "ray.h"
struct AABB {
    Vector3f mMin;
    Vector3f mMax;
    AABB():mMin(), mMax(){}
    AABB(const Vector3f& Min,const Vector3f& Max){
        for(int i=0;i < 3;i++){
           mMin[i] = Min[i];
           mMax[i] = Max[i];
        }
    }
    AABB(AABB const& a,const AABB& b) {
        for(int i=0;i < 3;i++){
            mMin[i] = std::min(a.mMin[i],b.mMin[i]);
            mMax[i] = std::max(a.mMax[i],b.mMax[i]);
        }
    }
    bool hit(const Ray& in,float t_min,float t_max) const {
        for(int i=0;i < 3;++i){
            float inv = 1.0f / in.direction()[i];
            float t0 = (mMin[i] - in.origin()[i]) * inv;
            float t1 = (mMax[i] - in.origin()[i]) * inv;
            if (inv < 0.0f)
                std::swap(t0,t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if(t_max <= t_min){
                return false;
            }
        }
        return true;
    }
    ~AABB() = default;
};
#endif //RAYTRACE_AABB_H
