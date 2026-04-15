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
using godot::ClassDB;
using godot::ObjectDB;
using godot::UtilityFunctions;
using godot::Object;
using godot::D_METHOD;
using godot::PropertyInfo;
using godot::PROPERTY_HINT_ENUM;
using godot::PROPERTY_USAGE_DEFAULT;
using godot::PROPERTY_USAGE_READ_ONLY;
using godot::PROPERTY_HINT_RESOURCE_TYPE;
using godot::CollisionShape3D;
using godot::SphereShape3D;
using godot::CylinderShape3D;
using godot::CapsuleShape3D;
using godot::BoxShape3D;
using godot::ConvexPolygonShape3D;
using godot::Transform3D;
using godot::Ref;
using godot::Resource;


void GeomOpsParams3D::set_transform(Transform3D const & p_transform)
{
    transform = p_transform;
}

Transform3D const & GeomOpsParams3D::get_transform() const
{
    return transform;
}

void GeomOpsParams3D::set_shape(Ref<Resource> const & p_shape)
{
    ERR_FAIL_COND(p_shape.is_null());
    shape = p_shape;
}

Ref<Resource> GeomOpsParams3D::get_shape() const
{
    return shape;
}

void GeomOpsParams3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("set_transform", "transform"), &GeomOpsParams3D::set_transform);
    ClassDB::bind_method(D_METHOD("get_transform"), &GeomOpsParams3D::get_transform);
    ClassDB::bind_method(D_METHOD("set_shape", "shape"), &GeomOpsParams3D::set_shape);
    ClassDB::bind_method(D_METHOD("get_shape"), &GeomOpsParams3D::get_shape);

    ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM3D, "transform"), "set_transform", "get_transform");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
}

godot::Vector3 const & GeomOpsResult3D::get_point_a() const
{
    return point_a;
}

godot::Vector3 const & GeomOpsResult3D::get_point_b() const
{
    return point_b;
}

void GeomOpsResult3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_point_a"), &GeomOpsResult3D::get_point_a);
    ClassDB::bind_method(D_METHOD("get_point_b"), &GeomOpsResult3D::get_point_b);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "point_a"), "", "get_point_a");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "point_b"), "", "get_point_b");
}


GeomOps3D * GeomOps3D::instance = nullptr;


void GeomOps3D::create_singleton()
{
    CRASH_COND_MSG(instance != nullptr, "GeomOps3D singleton is already created");
    instance = memnew(GeomOps3D);
}


void GeomOps3D::delete_singleton()
{
    CRASH_COND_MSG(instance == nullptr, "GeomOps3D singleton is already destroyed");
    memdelete(instance);
    instance = nullptr;
}


GeomOps3D * GeomOps3D::get_singleton()
{
    return instance;
}


void GeomOps3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("closest_to_shape", "params_a", "params_b", "result"), &GeomOps3D::closest_to_shape);
    ClassDB::bind_method(D_METHOD("closest_to_point", "params", "point", "result"), &GeomOps3D::closest_to_point);
    ClassDB::bind_method(D_METHOD("set_tolerance", "tolerance"), &GeomOps3D::set_tolerance);
    ClassDB::bind_method(D_METHOD("get_tolerance"), &GeomOps3D::get_tolerance);
    ClassDB::bind_method(D_METHOD("set_max_iter", "max_iter"), &GeomOps3D::set_max_iter);
    ClassDB::bind_method(D_METHOD("get_max_iter"), &GeomOps3D::get_max_iter);
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

Vector3 GeomOps3D::support(GeomOpsParams3D const * const p_params, godot::Vector3 const & p_dir)
{
    if (SphereShape3D const * const sphere = Object::cast_to<SphereShape3D>(*p_params->shape)) {
        return geomops::support(sphere, p_params->transform, p_dir);
    } else if (CapsuleShape3D const * const capsule = Object::cast_to<CapsuleShape3D>(*p_params->shape)) {
        return geomops::support(capsule, p_params->transform, p_dir);
    } else if (CylinderShape3D const * const cylinder = Object::cast_to<CylinderShape3D>(*p_params->shape)) {
        return geomops::support(cylinder, p_params->transform, p_dir);
    } else if (BoxShape3D const * const box = Object::cast_to<BoxShape3D>(*p_params->shape)) {
        return geomops::support(box, p_params->transform, p_dir);
    } else if (ConvexPolygonShape3D const * const convex = Object::cast_to<ConvexPolygonShape3D>(*p_params->shape)) {
        return geomops::support(convex, p_params->transform, p_dir);
    }
    UtilityFunctions::push_error("GeomOps: Unsupported shape type passed to support function.");
    return Vector3();
}

bool GeomOps3D::closest_to_shape(godot::Ref<GeomOpsParams3D> p_params_a,
                                 godot::Ref<GeomOpsParams3D> p_params_b,
                                 godot::Ref<GeomOpsResult3D> r_result)
{
    // if (!csa || !ObjectDB::get_instance(csa->get_instance_id())) {
    //     UtilityFunctions::push_error("The collision shape 'csa' is not valid.");
    //     return memnew(GeomOpsClosestPointPair3D());
    // }

    // if (!csb || !ObjectDB::get_instance(csb->get_instance_id())) {
    //     UtilityFunctions::push_error("The collision shape 'csb' is not valid.");        
    //     return memnew(GeomOpsClosestPointPair3D());
    // }

    // if (csa == csb) {
    //     UtilityFunctions::push_error("The same CollisionShape3D instance has been passed as both 'csa' and 'csb'.");
    //     return memnew(GeomOpsClosestPointPair3D());
    // }

    GeomOpsResult3D * const result = r_result.ptr();

    real_t const tolerance_squared = tolerance * tolerance;

    // Vector3 p = csb->get_global_position() - csa->get_global_position();
    Vector3 p = p_params_b->transform.origin - p_params_a->transform.origin;
    if (p.length_squared() < tolerance_squared) {
        UtilityFunctions::push_warning("GeomOps: Shapes are intersecting: the computed distance is zero or undefined.");
        return false;
    }

    real_t d = 0.0;

    Simplex simplex;
    Vector3 a = support(p_params_a.ptr(),  p);
    Vector3 b = support(p_params_b.ptr(), -p);
    Vector3 c = b - a;
    simplex.append(a, c);

    for (size_t i = 0; i < max_iter; i++) {
        p = simplex.get_closest_point(Vector3());

        d = p.length_squared();

        if (d < tolerance_squared) {
            UtilityFunctions::push_warning("GeomOps: Shapes are intersecting: the computed distance is zero or undefined.");
            return false;
        }

        simplex.reduce();
        
        a = support(p_params_a.ptr(),  p);
        b = support(p_params_b.ptr(), -p);
        c = b - a;

        if ((d - p.dot(c)) < tolerance) {
            result->point_a = simplex.get_closest_point_on_a();
            result->point_b = simplex.get_closest_point_on_b();
            return true;
        }

        simplex.append(a, c);
    }
    UtilityFunctions::push_warning("GeomOps: Maximum iteration limit reached: returning the latest approximated distance.");
    result->point_a = simplex.get_closest_point_on_a();
    result->point_b = simplex.get_closest_point_on_b();
    return true;
}

bool GeomOps3D::closest_to_point(godot::Ref<GeomOpsParams3D> p_params,
                                 godot::Vector3 p_point,
                                 godot::Ref<GeomOpsResult3D> r_result)
{
    // if (!cs || !ObjectDB::get_instance(cs->get_instance_id())) {
    //     UtilityFunctions::push_error("Invalid CollisionShape3D.");
    //     return memnew(GeomOpsClosestPointPair3D());
    // }

    GeomOpsResult3D * const result = r_result.ptr();

    real_t const tolerance_squared = tolerance * tolerance;

    Vector3 p = p_point - p_params->transform.origin;
    if (p.length_squared() < tolerance_squared) {
        return false;
    }

    real_t d = 0.0;

    Simplex simplex;
    Vector3 a = support(p_params.ptr(),  p);
    Vector3 b = p_point;
    Vector3 c = b - a;
    simplex.append(a, c);

    for (size_t i = 0; i < max_iter; i++) {

        p = simplex.get_closest_point(Vector3());
        d = p.length_squared();

        if (d < tolerance_squared) {
            UtilityFunctions::push_warning("GeomOps: Shapes are intersecting: the computed distance is zero or undefined.");
            return false;
        }

        simplex.reduce();

        a = support(p_params.ptr(),  p);
        b = p_point;
        c = b - a;

        if ((d - p.dot(c)) < tolerance) {
            result->point_a = simplex.get_closest_point_on_a();
            result->point_b = p_point;
            return true;
        }

        simplex.append(a, c);
    }

    UtilityFunctions::push_warning("GeomOps: Maximum iteration limit reached: returning the latest approximated distance.");
    result->point_a = simplex.get_closest_point_on_a();
    result->point_b = p_point;
    return true;
}


}
