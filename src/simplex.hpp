#ifndef DISTUTILS_SIMPLEX_HPP
#define DISTUTILS_SIMPLEX_HPP

#include <godot_cpp/variant/vector3.hpp>

using namespace godot;


class Simplex final
{
private:
    Vector3 subt[4];
    Vector3 diff[4];
    real_t bary[4];
    size_t size;

    static bool is_point_in_front_of_tetrahedron_face(Vector3 const a, Vector3 const b, 
                                                      Vector3 const c, Vector3 const d, 
                                                      Vector3 const p);

    static Vector3 get_closest_point_on_line(Vector3 const a, Vector3 const b, 
                                             Vector3 const p, real_t min_barycentric[]);

    static Vector3 get_closest_point_on_triangle(Vector3 const a, Vector3 const b, 
                                                 Vector3 const c, Vector3 const p, 
                                                 real_t min_barycentric[]);

    static Vector3 get_closest_point_on_tetrahedron(Vector3 const a, Vector3 const b, 
                                                    Vector3 const c, Vector3 const d, 
                                                    Vector3 const p, real_t min_barycentric[]);

    void remove(size_t const index);

public:
    Simplex();

    size_t get_size() const;
    void append(Vector3 const subtrahend, Vector3 const difference);
    void reduce();

    Vector3 get_closest_point(Vector3 const point);

    Vector3 get_closest_point_on_a() const;
    Vector3 get_closest_point_on_b() const;
};


#endif//DISTUTILS_SIMPLEX_HPP
