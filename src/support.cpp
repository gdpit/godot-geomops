#include "support.hpp"


namespace geomops {

using godot::Vector3;
using godot::Quaternion;
using godot::SphereShape3D;
using godot::CylinderShape3D;
using godot::CapsuleShape3D;
using godot::BoxShape3D;
using godot::ConvexPolygonShape3D;
using godot::PackedVector3Array;
using godot::SIGN;


Vector3 support(SphereShape3D const * shere, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction)
{
    return position + shere->get_radius() * direction.normalized();
}


Vector3 support(CapsuleShape3D const * capsule, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction)
{
    Vector3 const dir_local = orientation.xform_inv(direction);
    real_t const r = capsule->get_radius();
    Vector3 pnt_local(0.0, SIGN(dir_local.y) * (0.5 * capsule->get_height() - r), 0.0);
    pnt_local += r * dir_local.normalized();
    return position + orientation.xform(pnt_local);
}


Vector3 support(CylinderShape3D const * cylinder, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction)
{
    Vector3 const dir_local = orientation.xform_inv(direction);
    Vector3 const dir_radial(dir_local.x, 0.0, dir_local.z);
    Vector3 pnt_local(0.0, SIGN(dir_local.y) * 0.5 * cylinder->get_height(), 0.0);
    pnt_local += cylinder->get_radius() * dir_radial.normalized();
    return position + orientation.xform(pnt_local);
}


Vector3 support(BoxShape3D const * box, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction)
{
    Vector3 const dir_local = orientation.xform_inv(direction);
    return position + orientation.xform(dir_local.sign() * 0.5 * box->get_size());
}


Vector3 support(ConvexPolygonShape3D const * convex, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction)
{
    Vector3 const dir_local = orientation.xform_inv(direction);
    PackedVector3Array const pnt_array = convex->get_points();
    real_t max_value = std::numeric_limits<real_t>::lowest();
    int64_t max_index = -1;
    for (int64_t i = 0; i < pnt_array.size(); i++) {
        real_t const value = dir_local.dot(pnt_array[i]);
        if (value > max_value) {
            max_value = value;
            max_index = i;
        }
    }
    return position + orientation.xform(pnt_array[max_index]);
}

}
