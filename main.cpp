#include <iostream>
#include "vec.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "object.h"
#include "camera.h"
#include "Material.h"
#include <thread>
#include <sys/time.h>
#include "BVH.h"
Vector3f raytrace(const Ray& ray,const std::shared_ptr<Object>& world,int max_depth){
    if (max_depth <= 0)
        return Vector3f(0.0f);
    hit_record record;
    if (world->hit(ray,0.001,__FLT_MAX__,record)){
        Ray scatter;
        Vector3f color;
        if (record.material->scatter(ray,record,color,scatter))
        {
            return color * raytrace(scatter,world,max_depth-1);
        }
        return Vector3f(0.0f);
    }
    Vector3f color = ray.direction().normalize();
    float t = 0.5f * (color.y() + 1.0f);
    return t * Vector3f(0.5,0.7,1.0) + (1- t)*Vector3f(1.0f);
}
std::shared_ptr<Object> random_scene() {
    auto world = std::make_shared<hit_table>();
    auto checkTex = std::make_shared<ChessTexture>(.2,Vector3f(.2,.3,.1),Vector3f(.9,.9,.9));
    auto ground_material = std::make_shared<Lambertian>(checkTex);
    world->add(std::make_shared<Sphere>(Vector3f(0,-1000,0),1000,ground_material));

    for(int a=-11;a < 11;a++)
    {
        for(int b=-11 ;b < 11;b++)
        {
            auto material = random_float();
            Vector3f center(a + 0.9 * random_float() ,0.2 ,b + 0.9 * random_float());

            if ((center - Vector3f(4,0.2,0)).norm() >= 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (material < 0.8)
                {
                    auto color = Vector3f::random() * Vector3f::random();

                    sphere_material = std::make_shared<Lambertian>(color);
                    auto c2 =center + Vector3f(0, random_float(0.0,0.5),0.0);
                    world->add(std::make_shared<Sphere>(center,c2,0.2,sphere_material));
                }
                else if(material < 0.95)
                {
                    auto albedo = Vector3f::random(0.5,1.0f);
                    auto fuzz  = random_float(0,0.5);
                    sphere_material = std::make_shared<Metal>(albedo,fuzz);
                    world->add(std::make_shared<Sphere>(center,0.2,sphere_material));
                }
                else
                {
                    sphere_material = std::make_shared<Dielectric>(1.5f);
                    world->add(std::make_shared<Sphere>(center,0.2,sphere_material));
                }

            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    auto material2 = std::make_shared<Lambertian>(Vector3f(0.4,0.2,0.1));
    auto material3 = std::make_shared<Metal>(Vector3f(0.7,0.6,0.5),0);
    world->add(std::make_shared<Sphere>(Vector3f(0,1,0),1.0,material1));
    world->add(std::make_shared<Sphere>(Vector3f(-4,1,0),1.0,material2));
    world->add(std::make_shared<Sphere>(Vector3f(4,1,0),1.0,material3));
    auto bvh = std::make_shared<BVH>(world);
    return bvh;
}

int main(int argc,char *argv[])
{
    const int w = 1280;
    const int h = 960;
    const int comp = 3;
    const int samples = 16;
    std::array<uint8_t,w * h * comp> image{0};
    const int max_depth = 50;
    Camera cam(Vector3f(13,3,2.0f),Vector3f(0,0,0),Vector3f(0,1.0,0),60.f,w,h);
    auto world = random_scene();

    const int NUM_THREADS = 32;
    std::vector<std::thread> threads(NUM_THREADS);
    auto ROW_HEIGHT = h / NUM_THREADS;
    std::mutex mtx;
    int total = 0;
    for(int i=0;i < NUM_THREADS;++i){
        threads[i] = std::thread([&](int row_start,int row_end){
            for(int row = row_start;row < row_end;++row)
            {
                for(int col = 0;col < w;++col)
                {
                    Vector3f color(0.0f);
                    for(int s = 0;s < samples;++s) {
                        auto ray = cam.get_ray(col + random_float(), row +random_float());
                        color += raytrace(ray, world,max_depth);
                    }
                    color /= (float)samples;
                    int offset = (row * w + col) * comp;
                    image[offset + 0] = uint8_t(std::sqrtf(color.x())  * 255);
                    image[offset + 1] = uint8_t(std::sqrtf(color.y())  * 255);
                    image[offset + 2] = uint8_t(std::sqrtf(color.z()) * 255);
                }
                //
                {
                    std::lock_guard<std::mutex> lg(mtx);
                    total++;
                    fprintf(stdout,"\rRay tracing process: %.2f%%",static_cast<float>(total * 100.0f) / h);
                    fflush(stdout);
                }
            }

            },i*ROW_HEIGHT,(i+1)*ROW_HEIGHT);
    }
    for(auto&  t  : threads){
        if (t.joinable()) t.join();
    }
    fprintf(stdout,"\nDown     .");
    stbi_write_jpg("../ray.jpg",w,h,comp,image.data(),0);
    return 0;
}
