#ifndef DISTUTILS_SUPPORT_HPP
#define DISTUTILS_SUPPORT_HPP

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/cylinder_shape3d.hpp>
#include <godot_cpp/classes/box_shape3d.hpp>
#include <godot_cpp/classes/convex_polygon_shape3d.hpp>

using namespace godot;


Vector3 support(SphereShape3D const * shere, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction);

Vector3 support(CapsuleShape3D const * capsule, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction);

Vector3 support(CylinderShape3D const * cylinder, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction);

Vector3 support(BoxShape3D const * box, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction);

Vector3 support(ConvexPolygonShape3D const * convex, Vector3 const position, 
                Quaternion const orientation, Vector3 const direction);

#endif//DISTUTILS_SUPPORT_HPP
