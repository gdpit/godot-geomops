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


Vector3 support(SphereShape3D const * const sphere, Vector3 const & direction)
{
    return sphere->get_radius() * direction.normalized();
}


Vector3 support(CapsuleShape3D const * const capsule, Vector3 const & direction)
{
    real_t const r = capsule->get_radius();
    Vector3 max_point(0.0, SIGN(direction.y) * (0.5 * capsule->get_height() - r), 0.0);
    max_point += r * direction.normalized();
    return max_point;
}


Vector3 support(CylinderShape3D const * const cylinder, Vector3 const & direction)
{
    Vector3 const dir_radial(direction.x, 0.0, direction.z);
    Vector3 max_point(0.0, SIGN(direction.y) * 0.5 * cylinder->get_height(), 0.0);
    max_point += cylinder->get_radius() * dir_radial.normalized();
    return max_point;
}


Vector3 support(BoxShape3D const * const box, Vector3 const & direction)
{
    return direction.sign() * 0.5 * box->get_size();
}


Vector3 support(ConvexPolygonShape3D const * const convex, Vector3 const & direction)
{
    PackedVector3Array const & points = convex->get_points();

    if (points.is_empty()) {
        UtilityFunctions::push_error("GeomOps: support(): ConvexPolygonShape3D has no points. Invalid collision shape.");
        return Vector3();
    }
    
    Vector3 max_point = points[0];
    real_t max_value = direction.dot(max_point);
    
    for (int64_t i = 1; i < points.size(); i++) {
        Vector3 const & point = points[i];
        real_t const value = direction.dot(point);

        if (value > max_value) {
            max_value = value;
            max_point = point;
        }
    }
    return max_point;
}

}
