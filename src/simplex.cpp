#include "simplex.hpp"
#include <limits>


Simplex::Simplex()
: subt(), diff(), bary(), size(0)
{}


size_t Simplex::get_size() const
{
    return size;
}


bool Simplex::is_point_in_front_of_tetrahedron_face(Vector3 const a, Vector3 const b, 
                                                    Vector3 const c, Vector3 const d, 
                                                    Vector3 const p)
{
    Vector3 const n = vec3_cross(b - a, c - a);
    real_t const w = vec3_dot(d - a, n);
    if (ABS(w) < std::numeric_limits<real_t>::epsilon()) {
        puts("The point `d` is affinely dependent: the result is undetermined.");
    }
    return vec3_dot(p - a, n) * w < 0.0;
}


Vector3 Simplex::get_closest_point_on_line(Vector3 const a, Vector3 const b, 
                                           Vector3 const p, real_t min_barycentric[])
{
    Vector3 const ab = b - a;
    Vector3 const ap = p - a;
    real_t const t = CLAMP(vec3_dot(ab, ap) / vec3_dot(ab, ab), 0.0, 1.0);
    min_barycentric[0] = 1.0 - t;
    min_barycentric[1] = t;
    return a + (t * ab);
}


Vector3 Simplex::get_closest_point_on_triangle(Vector3 const a, Vector3 const b, 
                                               Vector3 const c, Vector3 const p, 
                                               real_t min_barycentric[])
{
    Vector3 const ab = b - a;
    Vector3 const ac = c - a;
    Vector3 const ap = p - a;
    real_t const d1 = vec3_dot(ab, ap);
    real_t const d2 = vec3_dot(ac, ap);
    if ((d1 <= 0.0) && (d2 <= 0.0)) {
        min_barycentric[0] = 1.0;
        return a;
    }
    Vector3 const bp = p - b;
    real_t const d3 = vec3_dot(ab, bp);
    real_t const d4 = vec3_dot(ac, bp);
    if ((d3 >= 0.0) && (d4 <= d3)){
        min_barycentric[1] = 1.0;
        return b;
    }
    real_t const vc = d1*d4 - d3*d2; 
    if ((vc <= 0.0) && (d1 >= 0.0) && (d3 <= 0.0)) {
        real_t const v = d1 / (d1 - d3);
        min_barycentric[0] = 1.0 - v;
        min_barycentric[1] = v;
        return a + v * ab;
    }
    Vector3 const cp = p - c;
    real_t const d5 = vec3_dot(ab, cp);
    real_t const d6 = vec3_dot(ac, cp);
    if ((d6 >= 0.0) && (d5 <= d6)) {
        min_barycentric[2] = 1.0;
        return c;
    }
    real_t const vb = d5*d2 - d1*d6;
    if ((vb <= 0.0) && (d2 >= 0.0) && (d6 <= 0.0)) {
        real_t const w = d2 / (d2 - d6);
        min_barycentric[0] = 1.0 - w;
        min_barycentric[2] = w;
        return a + w * ac;
    }
    real_t const va = d3*d6 - d5*d4;
    if ((va <= 0.0) && ((d4 - d3) >= 0.0) && ((d5 - d6) >= 0.0)) {
        real_t const w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        min_barycentric[1] = 1.0 - w;
        min_barycentric[2] = w;
        return b + w * (c - b);
    }
    real_t const denom = 1.0 / (va + vb + vc);
    real_t const v = vb * denom;
    real_t const w = vc * denom;
    min_barycentric[0] = 1.0 - v - w;
    min_barycentric[1] = v;
    min_barycentric[2] = w;
    return a + (v * ab) + (w * ac);
}


Vector3 Simplex::get_closest_point_on_tetrahedron(Vector3 const a, Vector3 const b, 
                                                  Vector3 const c, Vector3 const d, 
                                                  Vector3 const p, real_t min_barycentric[])
{
    Vector3 min_point = p;
    real_t min_value = std::numeric_limits<real_t>::max();
    if (is_point_in_front_of_tetrahedron_face(a, b, c, d, p)) {
        real_t barycentric[3] = {0};
        Vector3 const point = get_closest_point_on_triangle(a, b, c, p, barycentric);
        real_t const value = (point - p).length_squared();
        if (value < min_value) {
            min_value = value;
            min_point = point;
            min_barycentric[0] = barycentric[0];
            min_barycentric[1] = barycentric[1];
            min_barycentric[2] = barycentric[2];
        }
    }
    if (is_point_in_front_of_tetrahedron_face(a, c, d, b, p)) {
        real_t barycentric[3] = {0};
        Vector3 const point = get_closest_point_on_triangle(a, c, d, p, barycentric);
        real_t const value = (point - p).length_squared();
        if (value < min_value) {
            min_value = value;
            min_point = point;
            min_barycentric[0] = barycentric[0];
            min_barycentric[2] = barycentric[1];
            min_barycentric[3] = barycentric[2];
        }
    }
    if (is_point_in_front_of_tetrahedron_face(a, d, b, c, p)) {
        real_t barycentric[3] = {0};
        Vector3 const point = get_closest_point_on_triangle(a, d, b, p, barycentric);
        real_t const value = (point - p).length_squared();
        if (value < min_value) {
            min_value = value;
            min_point = point;
            min_barycentric[0] = barycentric[0];
            min_barycentric[3] = barycentric[1];
            min_barycentric[1] = barycentric[2];
        }
    }
    if (is_point_in_front_of_tetrahedron_face(b, d, c, a, p)) {
        real_t barycentric[3] = {0};
        Vector3 const point = get_closest_point_on_triangle(b, d, c, p, barycentric);
        real_t const value = (point - p).length_squared();
        if (value < min_value) {
            min_value = value;
            min_point = point;
            min_barycentric[1] = barycentric[0];
            min_barycentric[3] = barycentric[1];
            min_barycentric[2] = barycentric[2];
        }
    }
    return min_point;
}


void Simplex::remove(size_t const index)
{
    if (index > size) {
        puts("Index out of bounds: index exceeds the size of the simplex.");
        return;
    }
    size--;
    for (size_t i = index; i < size; i++) {
        subt[i] = subt[i + 1];
        diff[i] = diff[i + 1];
        bary[i] = bary[i + 1];
    }
    subt[size] = Vector3();
    diff[size] = Vector3();
    bary[size] = 0.0;
}


void Simplex::append(Vector3 const subtrahend, Vector3 const difference)
{
    if (size >= 4) {
        puts("Simplex has reached its maximum capacity: cannot add new point.");
        return;
    }
    subt[size] = subtrahend;
    diff[size] = difference;
    size++;
}


void Simplex::reduce()
{
    if ((size >= 4) && (bary[3] < std::numeric_limits<real_t>::epsilon())) {
        remove(3);
    }
    if ((size >= 3) && (bary[2] < std::numeric_limits<real_t>::epsilon())) {
        remove(2);
    }
    if ((size >= 2) && (bary[1] < std::numeric_limits<real_t>::epsilon())) {
        remove(1);
    }
    if ((size >= 1) && (bary[0] < std::numeric_limits<real_t>::epsilon())) {
        remove(0);
    }
}


Vector3 Simplex::get_closest_point(Vector3 const point)
{
    Vector3 cp = point;
    bary[0] = 0.0;
    bary[1] = 0.0;
    bary[2] = 0.0;
    bary[3] = 0.0;
    switch (size) {
        case 1: {
            bary[0] = 1.0;
            cp = diff[0];
        } break;
        case 2: {
            cp = get_closest_point_on_line(diff[0], diff[1], point, bary);
        } break;
        case 3: {
            cp = get_closest_point_on_triangle(diff[0], diff[1], diff[2], point, bary);
        } break;
        case 4: {
            cp = get_closest_point_on_tetrahedron(diff[0], diff[1], diff[2], diff[3], point, bary);
        } break;
    }
    return cp;
}


Vector3 Simplex::get_closest_point_on_a() const
{
    Vector3 cp;
    cp += bary[0] * (diff[0] + subt[0]);
    cp += bary[1] * (diff[1] + subt[1]);
    cp += bary[2] * (diff[2] + subt[2]);
    cp += bary[3] * (diff[3] + subt[3]);
    return cp;
}


Vector3 Simplex::get_closest_point_on_b() const
{
    Vector3 cp;
    cp += bary[0] * subt[0];
    cp += bary[1] * subt[1];
    cp += bary[2] * subt[2];
    cp += bary[3] * subt[3];
    return cp;
}

