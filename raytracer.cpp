#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <limits>

using namespace std;

const float INF = numeric_limits<float>::infinity();
const int WIDTH = 800;
const int HEIGHT = 600;

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xx) : x(xx), y(xx), z(xx) {}
    Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    Vec3 operator + (const Vec3 &v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator - (const Vec3 &v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator * (float f) const { return Vec3(x*f, y*f, z*f); }

    Vec3 normalize() const {
        float mag = sqrt(x*x + y*y + z*z);
        return *this * (1.0f / mag);
    }

    float dot(const Vec3 &v) const {
        return x*v.x + y*v.y + z*v.z;
    }
};

struct Ray {
    Vec3 origin, dir;
    Ray(Vec3 o, Vec3 d) : origin(o), dir(d) {}
};

struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(Vec3 c, float r, Vec3 col) : center(c), radius(r), color(col) {}

    bool intersect(const Ray &ray, float &t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.dir.dot(ray.dir);
        float b = 2 * oc.dot(ray.dir);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b*b - 4*a*c;

        if (discriminant < 0) return false;

        float t0 = (-b - sqrt(discriminant)) / (2*a);
        float t1 = (-b + sqrt(discriminant)) / (2*a);
        t = (t0 < t1) ? t0 : t1;

        return t > 0;
    }
};

// Clamp color to [0,255]
int clamp(float c) {
    return (int)(max(0.f, min(255.f, c)));
}

// Ray trace scene
Vec3 trace(const Ray &ray, const vector<Sphere> &spheres, const Vec3 &lightPos) {
    float tClosest = INF;
    const Sphere* hitSphere = nullptr;

    for (const auto& sphere : spheres) {
        float t;
        if (sphere.intersect(ray, t)) {
            if (t < tClosest) {
                tClosest = t;
                hitSphere = &sphere;
            }
        }
    }

    if (!hitSphere) return Vec3(30, 30, 30); // background color (dark gray)

    Vec3 hitPoint = ray.origin + ray.dir * tClosest;
    Vec3 normal = (hitPoint - hitSphere->center).normalize();
    Vec3 lightDir = (lightPos - hitPoint).normalize();

    float diffuse = max(0.0f, normal.dot(lightDir));
    Vec3 color = hitSphere->color * diffuse;

    return color;
}

int main() {
    ofstream img("output.ppm");
    img << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    vector<Sphere> spheres = {
        Sphere(Vec3(0, -1, 3), 1, Vec3(255, 0, 0)),     // Red sphere
        Sphere(Vec3(2, 0, 4), 1, Vec3(0, 0, 255)),      // Blue sphere
        Sphere(Vec3(-2, 0, 4), 1, Vec3(0, 255, 0))      // Green sphere
    };

    Vec3 lightPos(0, 5, 1);
    Vec3 camera(0, 0, 0);

    float fov = M_PI / 3.0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float px = (2 * (x + 0.5) / (float)WIDTH - 1) * tan(fov / 2.0) * WIDTH / (float)HEIGHT;
            float py = -(2 * (y + 0.5) / (float)HEIGHT - 1) * tan(fov / 2.0);
            Vec3 dir = Vec3(px, py, 1).normalize();
            Ray ray(camera, dir);
            Vec3 color = trace(ray, spheres, lightPos);
            img << clamp(color.x) << " " << clamp(color.y) << " " << clamp(color.z) << "\n";
        }
    }

    img.close();
    cout << "Image rendered to output.ppm" << endl;
    return 0;
}
