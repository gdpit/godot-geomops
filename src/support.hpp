#ifndef GEOMOPS_SUPPORT_HPP
#define GEOMOPS_SUPPORT_HPP

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>

namespace geomops {

godot::Vector3 support(godot::SphereShape3D const * const sphere, 
                       godot::Vector3 const & direction);

godot::Vector3 support(godot::CapsuleShape3D const * const capsule, 
                       godot::Vector3 const & direction);

godot::Vector3 support(godot::CylinderShape3D const * const cylinder, 
                       godot::Vector3 const & direction);

godot::Vector3 support(godot::BoxShape3D const * const box, 
                       godot::Vector3 const & direction);

godot::Vector3 support(godot::ConvexPolygonShape3D const * const convex, 
                       godot::Vector3 const & direction);

}

#endif//GEOMOPS_SUPPORT_HPP
