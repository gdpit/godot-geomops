#ifndef GEOMOPS_HPP
#define GEOMOPS_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

namespace geomops {

class GeomOpsClosestPointPair3D : public godot::RefCounted
{
    GDCLASS(GeomOpsClosestPointPair3D, RefCounted)
private:
    godot::Vector3 point_a;
    godot::Vector3 point_b;
protected:
    static void _bind_methods();
public:
    GeomOpsClosestPointPair3D();
    GeomOpsClosestPointPair3D(godot::Vector3 const pa, godot::Vector3 const pb);
    godot::Vector3 get_point_a() const;
    godot::Vector3 get_point_b() const;
};


class GeomOps3D: public godot::RefCounted
{
    GDCLASS(GeomOps3D, RefCounted)
private:
    static godot::real_t tolerance;
    static size_t max_iter;
    static godot::Vector3 support(godot::CollisionShape3D const * cs, godot::Vector3 const dir);
protected:
    static void _bind_methods();
public:
    GeomOps3D();
    static godot::real_t get_tolerance();
    static void set_tolerance(godot::real_t const t);
    static size_t get_max_iter();
    static void set_max_iter(size_t const n);
    static GeomOpsClosestPointPair3D * closest_shape(godot::CollisionShape3D const * csa, godot::CollisionShape3D const * csb);
    static GeomOpsClosestPointPair3D * closest_point(godot::CollisionShape3D const * cs, godot::Vector3 const q);
};

}

#endif//GEOMOPS_HPP
