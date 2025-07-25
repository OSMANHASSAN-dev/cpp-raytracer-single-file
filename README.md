# 🌟 C++ Single-File Ray Tracer

This is a minimal, self-contained ray tracer written in **pure C++**. It renders a simple 3D scene made of spheres and outputs a `.ppm` image using basic ray tracing principles — all inside a **single .cpp file**.

---

## ✨ Features

- ✅ Single-file design for easy learning and portability
- 🎯 Renders spheres with simple diffuse (Lambertian) shading
- 💡 Point light source casting soft shadows
- 🧮 Custom 3D vector math implementation
- 💾 Output in PPM image format (easy to open or convert)
- 🧱 No external libraries or dependencies

---

## 🖥️ How to Compile & Run

### On Windows (MinGW / Command Line)
```bash
g++ raytracer.cpp -o raytracer
./raytracer
