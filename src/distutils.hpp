#ifndef DISTUTILS_HPP
#define DISTUTILS_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

using namespace godot;


enum PrimitiveType
{
    PRIMITIVE_NONE = 0,
    PRIMITIVE_POINT = 1,
    PRIMITIVE_EDGE = 2,
    PRIMITIVE_TRIANGLE = 3
};

VARIANT_ENUM_CAST(PrimitiveType)


class DistanceDescriptor : public RefCounted 
{
    GDCLASS(DistanceDescriptor, RefCounted)
private:
    Vector3 point_a;
    Vector3 point_b;
    PrimitiveType type;
    real_t distance;
protected:
    static void _bind_methods();
public:
    DistanceDescriptor();
    DistanceDescriptor(PrimitiveType const t, Vector3 const pa, Vector3 const pb, real_t const d);
    PrimitiveType get_type() const;
    Vector3 get_point_a() const;
    Vector3 get_point_b() const;
    real_t get_distance() const;
};


class DistUtils: public RefCounted
{
    GDCLASS(DistUtils, RefCounted)
private:
    static real_t tolerance;
    static size_t max_iter;
protected:
    static void _bind_methods();
public:
    DistUtils();
    static real_t get_tolerance();
    static void set_tolerance(real_t const t);
    static size_t get_max_iter();
    static void set_max_iter(size_t const n);
    static Vector3 support(CollisionShape3D const * cs, Vector3 const dir);
    static DistanceDescriptor * distance_shape(CollisionShape3D const * csa, CollisionShape3D const * csb);
    static DistanceDescriptor * distance_point(CollisionShape3D const * cs, Vector3 const p);
};


#endif//DISTUTILS_HPP
