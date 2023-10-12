//
// Created by J SL on 2023/9/28.
//

#ifndef RAYTRACE_TEXTURE_H
#define RAYTRACE_TEXTURE_H
#include "vec.h"
class Texture {
public:
    virtual ~Texture() = default;
    virtual Vector3f get_color(Vector3f const& p)  const = 0;
};

class SoildTexture : public  Texture{
public:
    SoildTexture(Vector3f const& color): _color(color) {}
    virtual Vector3f get_color(Vector3f const& p)  const override{
        return _color;
    }
private:
    Vector3f _color;
};

class ChessTexture : public Texture{
public:
    ChessTexture(float scale,Vector3f const& even,const Vector3f& odd):
    _even(std::make_shared<SoildTexture>(even)),
    _odd(std::make_shared<SoildTexture>(odd)),
    _scale(1.0f / scale){}
    ChessTexture(float scale,std::shared_ptr<SoildTexture> const& even,const std::shared_ptr<SoildTexture>& odd):
        _even(even),_odd(odd),_scale(scale){}
    virtual Vector3f get_color(Vector3f const& p)  const override{
        int x = 0;
        for(int i=0;i <3;i++){
            x += static_cast<int>(std::floor(_scale * p[i]));
        }
        return x %2 == 0? _even->get_color(p) : _odd->get_color(p);
    }
private:
    std::shared_ptr<SoildTexture> _even;
    std::shared_ptr<SoildTexture> _odd;
    float _scale;
};
#endif //RAYTRACE_TEXTURE_H
