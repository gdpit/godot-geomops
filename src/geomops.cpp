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
using godot::Shape3D;


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

    ClassDB::bind_method(D_METHOD("node_closest_to_shape", "collision_shape_a", "collision_shape_b", "result"), &GeomOps3D::node_closest_to_shape);
    ClassDB::bind_method(D_METHOD("node_closest_to_point", "collision_shape", "point", "result"), &GeomOps3D::node_closest_to_point);

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
    Vector3 const direction = p_params->transform.basis.xform_inv(p_dir);
    Vector3 max_point;
    if (SphereShape3D const * const sphere = Object::cast_to<SphereShape3D>(*p_params->shape)) {
        max_point = geomops::support(sphere, direction);
    } else if (CapsuleShape3D const * const capsule = Object::cast_to<CapsuleShape3D>(*p_params->shape)) {
        max_point = geomops::support(capsule, direction);
    } else if (CylinderShape3D const * const cylinder = Object::cast_to<CylinderShape3D>(*p_params->shape)) {
        max_point = geomops::support(cylinder, direction);
    } else if (BoxShape3D const * const box = Object::cast_to<BoxShape3D>(*p_params->shape)) {
        max_point = geomops::support(box, direction);
    } else if (ConvexPolygonShape3D const * const convex = Object::cast_to<ConvexPolygonShape3D>(*p_params->shape)) {
        max_point = geomops::support(convex, direction);
    } else {
        UtilityFunctions::push_error("GeomOps: Unsupported shape type passed to support function.");
    }
    return p_params->transform.origin + p_params->transform.basis.xform(max_point);
}


bool GeomOps3D::closest_to_shape(godot::Ref<GeomOpsParams3D> p_params_a,
                                 godot::Ref<GeomOpsParams3D> p_params_b,
                                 godot::Ref<GeomOpsResult3D> r_result)
{
    ERR_FAIL_COND_V(p_params_a.is_null(), false);
    ERR_FAIL_COND_V(p_params_b.is_null(), false);

    ERR_FAIL_COND_V(p_params_a->shape.is_null(), false);
    ERR_FAIL_COND_V(p_params_b->shape.is_null(), false);
    
    GeomOpsResult3D * const result = r_result.ptr();

    real_t const tolerance_squared = tolerance * tolerance;

    Vector3 p = p_params_b->transform.origin - p_params_a->transform.origin;
    if (p.length_squared() < tolerance_squared) {
        UtilityFunctions::push_warning("GeomOps::closest_to_shape(): Shapes are intersecting: the computed distance is zero or undefined.");
        return false;
    }

    if (r_result.is_null()) {
        r_result.instantiate();
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
            UtilityFunctions::push_warning("GeomOps::closest_to_shape(): Shapes are intersecting: the computed distance is zero or undefined.");
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
    UtilityFunctions::push_warning("GeomOps::closest_to_shape(): Maximum iteration limit reached: returning the latest approximated distance.");
    result->point_a = simplex.get_closest_point_on_a();
    result->point_b = simplex.get_closest_point_on_b();
    return true;
}


bool GeomOps3D::closest_to_point(godot::Ref<GeomOpsParams3D> p_params,
                                 godot::Vector3 p_point,
                                 godot::Ref<GeomOpsResult3D> r_result)
{
    ERR_FAIL_COND_V(p_params.is_null(), false);

    ERR_FAIL_COND_V(p_params->shape.is_null(), false);
    
    GeomOpsResult3D * const result = r_result.ptr();

    real_t const tolerance_squared = tolerance * tolerance;

    Vector3 p = p_point - p_params->transform.origin;
    if (p.length_squared() < tolerance_squared) {
        UtilityFunctions::push_warning("GeomOps::closest_to_point(): Shapes are intersecting: the computed distance is zero or undefined.");
        return false;
    }

    if (r_result.is_null()) {
        r_result.instantiate();
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
            UtilityFunctions::push_warning("GeomOps::closest_to_point(): Shapes are intersecting: the computed distance is zero or undefined.");
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

    UtilityFunctions::push_warning("GeomOps::closest_to_point(): Maximum iteration limit reached: returning the latest approximated distance.");
    result->point_a = simplex.get_closest_point_on_a();
    result->point_b = p_point;
    return true;
}


bool GeomOps3D::node_closest_to_shape(CollisionShape3D const * p_collision_shape_a,
                                      CollisionShape3D const * p_collision_shape_b,
                                      Ref<GeomOpsResult3D> r_result)
{
    ERR_FAIL_NULL_V(p_collision_shape_a, false);
    ERR_FAIL_NULL_V(p_collision_shape_b, false);

    if (p_collision_shape_a == p_collision_shape_b) {
        UtilityFunctions::push_error("GeomOps::node_closest_to_shape(): Both inputs reference the same CollisionShape3D instance.");
        return false;
    }
    
    Ref<GeomOpsParams3D> params_a;
    params_a.instantiate();
    params_a->shape = p_collision_shape_a->get_shape();
    params_a->transform = p_collision_shape_a->get_global_transform();
    
    Ref<GeomOpsParams3D> params_b;
    params_b.instantiate();
    params_b->shape = p_collision_shape_b->get_shape();
    params_b->transform = p_collision_shape_b->get_global_transform();

    return closest_to_shape(params_a, params_b, r_result);
}


bool GeomOps3D::node_closest_to_point(CollisionShape3D const * p_collision_shape,
                                      Vector3 p_point,
                                      Ref<GeomOpsResult3D> r_result)
{
    ERR_FAIL_NULL_V(p_collision_shape, false);

    Ref<GeomOpsParams3D> params;
    params.instantiate();
    params->shape = p_collision_shape->get_shape();
    params->transform = p_collision_shape->get_global_transform();

    return closest_to_point(params, p_point, r_result);
}


}
