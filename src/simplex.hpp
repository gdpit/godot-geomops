#ifndef GEOMOPS_SIMPLEX_HPP
#define GEOMOPS_SIMPLEX_HPP

#include <godot_cpp/variant/vector3.hpp>

namespace geomops {

class Simplex final
{
private:
    godot::Vector3 subt[4];
    godot::Vector3 diff[4];
    godot::real_t bary[4];
    size_t size;

    static bool is_point_in_front_of_tetrahedron_face(
        godot::Vector3 const a, godot::Vector3 const b, 
        godot::Vector3 const c, godot::Vector3 const d, 
        godot::Vector3 const p);

    static godot::Vector3 get_closest_point_on_line(
        godot::Vector3 const a, godot::Vector3 const b, 
        godot::Vector3 const p, godot::real_t min_barycentric[]);

    static godot::Vector3 get_closest_point_on_triangle(
        godot::Vector3 const a, godot::Vector3 const b, 
        godot::Vector3 const c, godot::Vector3 const p, 
        godot::real_t min_barycentric[]);

    static godot::Vector3 get_closest_point_on_tetrahedron(
        godot::Vector3 const a, godot::Vector3 const b, 
        godot::Vector3 const c, godot::Vector3 const d, 
        godot::Vector3 const p, godot::real_t min_barycentric[]);

    void remove(size_t const index);

public:
    Simplex();

    size_t get_size() const;
    void append(godot::Vector3 const subtrahend, godot::Vector3 const difference);
    void reduce();

    godot::Vector3 get_closest_point(godot::Vector3 const point);

    godot::Vector3 get_closest_point_on_a() const;
    godot::Vector3 get_closest_point_on_b() const;
};

}

#endif//GEOMOPS_SIMPLEX_HPP
