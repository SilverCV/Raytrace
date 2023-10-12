//
// Created by J SL on 2023/9/26.
//

#ifndef RAYTRACE_BVH_H
#define RAYTRACE_BVH_H
#include "object.h"
#include "AABB.h"
#include <functional>
static inline bool compator(const std::shared_ptr<Object>& a,const std::shared_ptr<Object>& b,int axis) {
    return a->bounding_box().mMin[axis] < b->bounding_box().mMin[axis];
}
class BVH : public Object{
public:
    BVH(std::shared_ptr<hit_table> const& table): BVH(table->Objects(),0,table->Objects().size()){}
    BVH(const std::vector<std::shared_ptr<Object>> const& object,int start,int end){
        auto objects = object;
        auto axis = rand()%3;
        auto comp = std::bind(compator,std::placeholders::_1,std::placeholders::_2,axis);
        auto size = end - start;
        if (size == 1){
            _left = objects[start];
            _right = objects[start];
        }
        else if (size == 2) {
            if (comp(objects[start],objects[start+1])) {
                _left = objects[start];
                _right = objects[start+1];
            }
            else{
                _left = objects[start+1];
                _right = objects[start];
            }
        }else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comp);
            auto middle = start + size / 2;
            _left = std::make_shared<BVH>(objects, start, middle);
            _right = std::make_shared<BVH>(objects, middle , end);

        }
        _box = AABB(_left->bounding_box(),_right->bounding_box());
    }
    virtual AABB bounding_box() const override {return _box;};
    virtual bool hit(const Ray& in,float t_min,float t_max,hit_record& rec) const override{
        if (!_box.hit(in,t_min,t_max)){
            return false;
        }

        bool hit_left = _left->hit(in,t_min,t_max,rec);
        bool  hit_right = _right->hit(in,t_min,hit_left ? rec.t : t_max,rec);

        return hit_left || hit_right;
    }
private:
    std::shared_ptr<Object> _left;
    std::shared_ptr<Object> _right;
    AABB _box;
};
#endif //RAYTRACE_BVH_H
