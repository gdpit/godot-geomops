#include "distutils.hpp"
#include "simplex.hpp"

#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;


DistanceDescriptor::DistanceDescriptor()
: type(PRIMITIVE_NONE), point_a(), point_b(), distance(0.0)
{}


DistanceDescriptor::DistanceDescriptor(PrimitiveType const t, Vector3 const pa, Vector3 const pb, real_t const d)
: type(t), point_a(pa), point_b(pb), distance(d)
{}


void DistanceDescriptor::_bind_methods()
{
    BIND_ENUM_CONSTANT(PRIMITIVE_NONE);
    BIND_ENUM_CONSTANT(PRIMITIVE_POINT);
    BIND_ENUM_CONSTANT(PRIMITIVE_EDGE);
    BIND_ENUM_CONSTANT(PRIMITIVE_TRIANGLE);

    ClassDB::bind_method(D_METHOD("get_type"), &DistanceDescriptor::get_type);
    ClassDB::bind_method(D_METHOD("get_point_a"), &DistanceDescriptor::get_point_a);
    ClassDB::bind_method(D_METHOD("get_point_b"), &DistanceDescriptor::get_point_b);
    ClassDB::bind_method(D_METHOD("get_distance"), &DistanceDescriptor::get_distance);

    ClassDB::add_property("DistanceDescriptor", PropertyInfo(Variant::INT, "type", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_type");
    ClassDB::add_property("DistanceDescriptor", PropertyInfo(Variant::VECTOR3, "point_a", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_point_a");
    ClassDB::add_property("DistanceDescriptor", PropertyInfo(Variant::VECTOR3, "point_b", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_point_b");
    ClassDB::add_property("DistanceDescriptor", PropertyInfo(Variant::FLOAT, "distance", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_distance");
}


PrimitiveType DistanceDescriptor::get_type() const
{
    return type;
}


Vector3 DistanceDescriptor::get_point_a() const
{
    return point_a;
}


Vector3 DistanceDescriptor::get_point_b() const
{
    return point_b;
}


real_t DistanceDescriptor::get_distance() const
{
    return distance;
}


real_t DistUtils::tolerance = 1e-3;
size_t DistUtils::max_iter  = 128;


void DistUtils::_bind_methods()
{
    ClassDB::bind_static_method("DistUtils", D_METHOD("support", "cs", "dir"), &DistUtils::support);
    ClassDB::bind_static_method("DistUtils", D_METHOD("distance_shape", "csa", "csb"), &DistUtils::distance_shape);
    ClassDB::bind_static_method("DistUtils", D_METHOD("distance_point", "cs", "p"), &DistUtils::distance_point);
    ClassDB::bind_static_method("DistUtils", D_METHOD("get_tolerance"), &DistUtils::get_tolerance);
    ClassDB::bind_static_method("DistUtils", D_METHOD("set_tolerance", "tolerance"), &DistUtils::set_tolerance);
    ClassDB::bind_static_method("DistUtils", D_METHOD("get_max_iter"), &DistUtils::get_max_iter);
    ClassDB::bind_static_method("DistUtils", D_METHOD("set_max_iter", "max_iter"), &DistUtils::set_max_iter);
}


real_t DistUtils::get_tolerance()
{
    return tolerance;
}


void DistUtils::set_tolerance(real_t const t)
{
    tolerance = t;
}


size_t DistUtils::get_max_iter()
{
    return max_iter;
}


void DistUtils::set_max_iter(size_t const n)
{
    max_iter = n;
}



DistUtils::DistUtils()
{}


Vector3 DistUtils::support(CollisionShape3D const * cs, Vector3 const dir)
{
    Vector3 const pos = cs->get_global_position();
    Quaternion const ort = Quaternion(cs->get_global_rotation());
    if (SphereShape3D const * sphere = Object::cast_to<SphereShape3D>(*cs->get_shape())) {
        return pos + sphere->get_radius() * dir.normalized();
    } else if (CapsuleShape3D const * capsule = Object::cast_to<CapsuleShape3D>(*cs->get_shape())) {
        Vector3 const dir_local = ort.xform_inv(dir);
        real_t const r = capsule->get_radius();
        Vector3 pnt_local(0.0, SIGN(dir_local.y) * (0.5 * capsule->get_height() - r), 0.0);
        pnt_local += r * dir_local.normalized();
        return pos + ort.xform(pnt_local);
    } else if (CylinderShape3D const * cylinder = Object::cast_to<CylinderShape3D>(*cs->get_shape())) {
        Vector3 const dir_local = ort.xform_inv(dir);
        Vector3 const dir_radial(dir_local.x, 0.0, dir_local.z);
        Vector3 pnt_local(0.0, SIGN(dir_local.y) * 0.5 * cylinder->get_height(), 0.0);
        pnt_local += cylinder->get_radius() * dir_radial.normalized();
        return pos + ort.xform(pnt_local);
    } else if (BoxShape3D const * box = Object::cast_to<BoxShape3D>(*cs->get_shape())) {
        Vector3 const dir_local = ort.xform_inv(dir);
        return pos + ort.xform(dir_local.sign() * 0.5 * box->get_size());
    } else if (ConvexPolygonShape3D const * convex = Object::cast_to<ConvexPolygonShape3D>(*cs->get_shape())) {
        Vector3 const dir_local = ort.xform_inv(dir);
        PackedVector3Array const pnt_array = dynamic_cast<ConvexPolygonShape3D const*>(convex)->get_points();
        real_t max_value = std::numeric_limits<real_t>::lowest();
        int64_t max_index = -1;
        for (int64_t i = 0; i < pnt_array.size(); i++) {
            real_t const value = dir_local.dot(pnt_array[i]);
            if (value > max_value) {
                max_value = value;
                max_index = i;
            }
        }
        return pos + ort.xform(pnt_array[max_index]);
    }
    UtilityFunctions::printerr("Not Implemented");
    return Vector3();
}


DistanceDescriptor * DistUtils::distance_shape(CollisionShape3D const * csa, CollisionShape3D const * csb)
{
    if (csa == csb) {
        UtilityFunctions::printerr("'a' and 'b' point to the same collision shape.");
        return memnew(DistanceDescriptor());
    }

    Vector3 p = csb->get_global_position() - csa->get_global_position();

    Simplex simplex;
    Vector3 a = support(csa,  p);
    Vector3 b = support(csb, -p);
    Vector3 c = b - a;
    simplex.append(a, c);

    for (size_t i = 0; i < max_iter; i++) {
        p = simplex.get_closest_point(Vector3());
        
        real_t const d = p.dot(p);

        if (d < (tolerance * tolerance)) {
            return memnew(DistanceDescriptor());
        }

        simplex.reduce();
        
        a = support(csa,  p);
        b = support(csb, -p);
        c = b - a;

        if ((d - p.dot(c)) < tolerance) {
            PrimitiveType const t = static_cast<PrimitiveType>(simplex.get_size());
            Vector3 const cpa = simplex.get_closest_point_on_a();
            Vector3 const cpb = simplex.get_closest_point_on_b();
            return memnew(DistanceDescriptor(t, cpa, cpb, sqrt(d)));
        }

        simplex.append(a, c);
    }
    return memnew(DistanceDescriptor());
}


DistanceDescriptor * DistUtils::distance_point(CollisionShape3D const * cs, Vector3 const p)
{
    UtilityFunctions::printerr("Maximum iteration count reached, output is not valid.");
    return memnew(DistanceDescriptor());
}
