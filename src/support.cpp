#include "support.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

namespace geomops {

using godot::Vector3;
using godot::Transform3D;
using godot::SphereShape3D;
using godot::CylinderShape3D;
using godot::CapsuleShape3D;
using godot::BoxShape3D;
using godot::ConvexPolygonShape3D;
using godot::PackedVector3Array;
using godot::SIGN;
using godot::UtilityFunctions;


Vector3 support(SphereShape3D const * const sphere, 
                Transform3D const & transform, 
                Vector3 const & direction)
{
    return transform.origin + sphere->get_radius() * direction.normalized();
}


Vector3 support(CapsuleShape3D const * const capsule, 
                Transform3D const & transform, 
                Vector3 const & direction)
{
    Vector3 const dir_local = transform.basis.xform_inv(direction);
    real_t const r = capsule->get_radius();
    Vector3 pnt_local(0.0, SIGN(dir_local.y) * (0.5 * capsule->get_height() - r), 0.0);
    pnt_local += r * dir_local.normalized();
    return transform.origin + transform.basis.xform(pnt_local);

}


Vector3 support(CylinderShape3D const * const cylinder, 
                Transform3D const & transform, 
                Vector3 const & direction)
{
    Vector3 const dir_local = transform.basis.xform_inv(direction);
    Vector3 const dir_radial(dir_local.x, 0.0, dir_local.z);
    Vector3 pnt_local(0.0, SIGN(dir_local.y) * 0.5 * cylinder->get_height(), 0.0);
    pnt_local += cylinder->get_radius() * dir_radial.normalized();
    return transform.origin + transform.basis.xform(pnt_local);
}


Vector3 support(BoxShape3D const * const box, 
                Transform3D const & transform, 
                Vector3 const & direction)
{
    Vector3 const dir_local = transform.basis.xform_inv(direction);
    return transform.origin + transform.basis.xform(dir_local.sign() * 0.5 * box->get_size());
}


Vector3 support(ConvexPolygonShape3D const * const convex, 
                Transform3D const & transform, 
                Vector3 const & direction)
{
    PackedVector3Array const & points = convex->get_points();

    if (points.is_empty()) {
        UtilityFunctions::push_error("GeomOps: support(): ConvexPolygonShape3D has no points. Invalid collision shape.");
        return transform.origin;
    }

    Vector3 const dir_local = transform.basis.xform_inv(direction);
    
    Vector3 max_point = points[0];
    real_t max_value = dir_local.dot(max_point);
    
    for (int64_t i = 1; i < points.size(); i++) {
        Vector3 const & point = points[i];
        real_t const value = dir_local.dot(point);

        if (value > max_value) {
            max_value = value;
            max_point = point;
        }
    }
    return transform.origin + transform.basis.xform(max_point);
}

}
