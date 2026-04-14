#include "geomops.hpp"
#include "simplex.hpp"
#include "support.hpp"

#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace geomops {

using godot::real_t;
using godot::Variant;
using godot::Vector3;
using godot::Quaternion;
using godot::ClassDB;
using godot::ObjectDB;
using godot::UtilityFunctions;
using godot::Object;
using godot::D_METHOD;
using godot::PropertyInfo;
using godot::PROPERTY_HINT_ENUM;
using godot::PROPERTY_USAGE_DEFAULT;
using godot::PROPERTY_USAGE_READ_ONLY;
using godot::CollisionShape3D;
using godot::SphereShape3D;
using godot::CylinderShape3D;
using godot::CapsuleShape3D;
using godot::BoxShape3D;
using godot::ConvexPolygonShape3D;


GeomOpsClosestPointPair3D::GeomOpsClosestPointPair3D()
:  point_a(), point_b()
{}


GeomOpsClosestPointPair3D::GeomOpsClosestPointPair3D(Vector3 const pa, Vector3 const pb)
: point_a(pa), point_b(pb)
{}


void GeomOpsClosestPointPair3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_point_a"), &GeomOpsClosestPointPair3D::get_point_a);
    ClassDB::bind_method(D_METHOD("get_point_b"), &GeomOpsClosestPointPair3D::get_point_b);

    ClassDB::add_property("GeomOpsClosestPointPair3D", PropertyInfo(Variant::VECTOR3, "point_a", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_point_a");
    ClassDB::add_property("GeomOpsClosestPointPair3D", PropertyInfo(Variant::VECTOR3, "point_b", PROPERTY_HINT_ENUM, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_READ_ONLY), "", "get_point_b");
}


Vector3 GeomOpsClosestPointPair3D::get_point_a() const
{
    return point_a;
}


Vector3 GeomOpsClosestPointPair3D::get_point_b() const
{
    return point_b;
}


real_t GeomOps3D::tolerance = 1e-3;
size_t GeomOps3D::max_iter  = 128;


void GeomOps3D::_bind_methods()
{
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("closest_shape", "csa", "csb"), &GeomOps3D::closest_shape);
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("closest_point", "cs", "p"), &GeomOps3D::closest_point);
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("get_tolerance"), &GeomOps3D::get_tolerance);
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("set_tolerance", "tolerance"), &GeomOps3D::set_tolerance);
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("get_max_iter"), &GeomOps3D::get_max_iter);
    ClassDB::bind_static_method("GeomOps3D", D_METHOD("set_max_iter", "max_iter"), &GeomOps3D::set_max_iter);
}


real_t GeomOps3D::get_tolerance()
{
    return tolerance;
}


void GeomOps3D::set_tolerance(real_t const t)
{
    tolerance = t;
}


size_t GeomOps3D::get_max_iter()
{
    return max_iter;
}


void GeomOps3D::set_max_iter(size_t const n)
{
    max_iter = n;
}



GeomOps3D::GeomOps3D()
{}


Vector3 GeomOps3D::support(CollisionShape3D const * cs, Vector3 const dir)
{
    Vector3 const pos = cs->get_global_position();
    Quaternion const ort = Quaternion::from_euler(cs->get_global_rotation());
    if (SphereShape3D const * sphere = Object::cast_to<SphereShape3D>(*cs->get_shape())) {
        return geomops::support(sphere, pos, ort, dir);
    } else if (CapsuleShape3D const * capsule = Object::cast_to<CapsuleShape3D>(*cs->get_shape())) {
        return geomops::support(capsule, pos, ort, dir);
    } else if (CylinderShape3D const * cylinder = Object::cast_to<CylinderShape3D>(*cs->get_shape())) {
        return geomops::support(cylinder, pos, ort, dir);
    } else if (BoxShape3D const * box = Object::cast_to<BoxShape3D>(*cs->get_shape())) {
        return geomops::support(box, pos, ort, dir);
    } else if (ConvexPolygonShape3D const * convex = Object::cast_to<ConvexPolygonShape3D>(*cs->get_shape())) {
        return geomops::support(convex, pos, ort, dir);
    }
    UtilityFunctions::push_error("Unsupported shape type passed to support function.");
    return Vector3();
}


GeomOpsClosestPointPair3D * GeomOps3D::closest_shape(CollisionShape3D const * csa, CollisionShape3D const * csb)
{
    if (!csa || !ObjectDB::get_instance(csa->get_instance_id())) {
        UtilityFunctions::push_error("The collision shape 'csa' is not valid.");
        return memnew(GeomOpsClosestPointPair3D());
    }

    if (!csb || !ObjectDB::get_instance(csb->get_instance_id())) {
        UtilityFunctions::push_error("The collision shape 'csb' is not valid.");        
        return memnew(GeomOpsClosestPointPair3D());
    }

    if (csa == csb) {
        UtilityFunctions::push_error("The same CollisionShape3D instance has been passed as both 'csa' and 'csb'.");
        return memnew(GeomOpsClosestPointPair3D());
    }

    real_t const tolerance_squared = tolerance * tolerance;

    Vector3 p = csb->get_global_position() - csa->get_global_position();
    if (p.length_squared() < tolerance_squared) {
        p = Vector3(1.0f, 0.0f, 0.0f);
    }

    real_t d = 0.0;

    Simplex simplex;
    Vector3 a = support(csa,  p);
    Vector3 b = support(csb, -p);
    Vector3 c = b - a;
    simplex.append(a, c);

    for (size_t i = 0; i < max_iter; i++) {
        p = simplex.get_closest_point(Vector3());

        d = p.length_squared();

        if (d < tolerance_squared) {
            UtilityFunctions::push_warning("Shapes are intersecting: the computed distance is zero or undefined.");
            return memnew(GeomOpsClosestPointPair3D());
        }

        simplex.reduce();
        
        a = support(csa,  p);
        b = support(csb, -p);
        c = b - a;

        if ((d - p.dot(c)) < tolerance) {
            Vector3 const cpa = simplex.get_closest_point_on_a();
            Vector3 const cpb = simplex.get_closest_point_on_b();
            return memnew(GeomOpsClosestPointPair3D(cpa, cpb));
        }

        simplex.append(a, c);
    }
    UtilityFunctions::push_warning("Maximum iteration limit reached: returning the latest approximated distance.");
    Vector3 const cpa = simplex.get_closest_point_on_a();
    Vector3 const cpb = simplex.get_closest_point_on_b();
    return memnew(GeomOpsClosestPointPair3D(cpa, cpb));
}


GeomOpsClosestPointPair3D * GeomOps3D::closest_point(CollisionShape3D const * cs, Vector3 const q)
{
    if (!cs || !ObjectDB::get_instance(cs->get_instance_id())) {
        UtilityFunctions::push_error("Invalid CollisionShape3D.");
        return memnew(GeomOpsClosestPointPair3D());
    }

    real_t const tolerance_squared = tolerance * tolerance;

    Vector3 p = q - cs->get_global_position();
    if (p.length_squared() < tolerance_squared) {
        return memnew(GeomOpsClosestPointPair3D());
    }

    real_t d = 0.0;

    Simplex simplex;
    Vector3 a = support(cs,  p);
    Vector3 b = q;
    Vector3 c = b - a;
    simplex.append(a, c);

    for (size_t i = 0; i < max_iter; i++) {

        p = simplex.get_closest_point(Vector3());
        d = p.length_squared();

        if (d < tolerance_squared) {
            return memnew(GeomOpsClosestPointPair3D());
        }

        simplex.reduce();

        a = support(cs,  p);
        b = q;
        c = b - a;

        if ((d - p.dot(c)) < tolerance) {
            break;
        }

        simplex.append(a, c);
    }

    Vector3 const cpa = simplex.get_closest_point_on_a();
    Vector3 const cpb = q;

    return memnew(GeomOpsClosestPointPair3D(cpa, cpb));
}


}












