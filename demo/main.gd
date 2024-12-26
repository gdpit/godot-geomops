@tool
extends Node3D

@onready
var shapes: Array[CollisionShape3D] = [
	$Sphere/CollisionShape3D, 
	$Capsule/CollisionShape3D, 
	$Cylinder/CollisionShape3D, 
	$Box/CollisionShape3D, 
	$Convex/CollisionShape3D
]


func _ready() -> void:
	DistUtils.set_tolerance(1e-4)
	
	#$Convex/CollisionShape3D.shape.points = PackedVector3Array([
		#Vector3( 0.000000, -1.000000, -1.000000),
		#Vector3( 0.707107, -1.000000, -0.707107),
		#Vector3( 1.000000, -1.000000,  0.000000),
		#Vector3( 0.707107, -1.000000,  0.707107),
		#Vector3( 0.000000, -1.000000,  1.000000),
		#Vector3(-0.707107, -1.000000,  0.707107),
		#Vector3(-1.000000, -1.000000,  0.000000),
		#Vector3(-0.707107, -1.000000, -0.707107),
		#Vector3( 0.000000,  1.000000,  0.000000),
	#])
	
	var bodies :=  get_children().filter(func(node): return node is StaticBody3D)
	var count := bodies.size()
	var angle := TAU / float(count)
	for i in count:
		bodies[i].position = 5.0 * Vector3.FORWARD.rotated(Vector3.UP, float(i) * angle)


@warning_ignore("unused_parameter")
func _physics_process(delta: float) -> void:
	var sphere_capsule_dist_desc    := DistUtils.distance_shape($Sphere/CollisionShape3D, $Capsule/CollisionShape3D)
	var sphere_cylinder_dist_desc   := DistUtils.distance_shape($Sphere/CollisionShape3D, $Cylinder/CollisionShape3D)
	var sphere_box_dist_desc        := DistUtils.distance_shape($Sphere/CollisionShape3D, $Box/CollisionShape3D)
	var sphere_convex_dist_desc     := DistUtils.distance_shape($Sphere/CollisionShape3D, $Convex/CollisionShape3D)
	var capsule_cylinder_dist_desc  := DistUtils.distance_shape($Capsule/CollisionShape3D, $Cylinder/CollisionShape3D)
	var capsule_box_dist_desc       := DistUtils.distance_shape($Capsule/CollisionShape3D, $Box/CollisionShape3D)
	var capsule_convex_dist_desc    := DistUtils.distance_shape($Capsule/CollisionShape3D, $Convex/CollisionShape3D)
	var cylinder_box_dist_desc      := DistUtils.distance_shape($Cylinder/CollisionShape3D, $Box/CollisionShape3D)
	var cylinder_convex_dist_desc   := DistUtils.distance_shape($Cylinder/CollisionShape3D, $Convex/CollisionShape3D)
	var box_convex_dist_desc        := DistUtils.distance_shape($Box/CollisionShape3D, $Convex/CollisionShape3D)
	
	$ClosestPoints/SphereCapsule/PointA.position   = sphere_capsule_dist_desc.point_a
	$ClosestPoints/SphereCapsule/PointB.position   = sphere_capsule_dist_desc.point_b
	$ClosestPoints/SphereCylinder/PointA.position  = sphere_cylinder_dist_desc.point_a
	$ClosestPoints/SphereCylinder/PointB.position  = sphere_cylinder_dist_desc.point_b
	$ClosestPoints/SphereBox/PointA.position       = sphere_box_dist_desc.point_a
	$ClosestPoints/SphereBox/PointB.position       = sphere_box_dist_desc.point_b
	$ClosestPoints/SphereConvex/PointA.position    = sphere_convex_dist_desc.point_a
	$ClosestPoints/SphereConvex/PointB.position    = sphere_convex_dist_desc.point_b
	$ClosestPoints/CapsuleCylinder/PointA.position = capsule_cylinder_dist_desc.point_a
	$ClosestPoints/CapsuleCylinder/PointB.position = capsule_cylinder_dist_desc.point_b
	$ClosestPoints/CapsuleBox/PointA.position      = capsule_box_dist_desc.point_a
	$ClosestPoints/CapsuleBox/PointB.position      = capsule_box_dist_desc.point_b
	$ClosestPoints/CapsuleConvex/PointA.position   = capsule_convex_dist_desc.point_a
	$ClosestPoints/CapsuleConvex/PointB.position   = capsule_convex_dist_desc.point_b
	$ClosestPoints/CylinderBox/PointA.position     = cylinder_box_dist_desc.point_a
	$ClosestPoints/CylinderBox/PointB.position     = cylinder_box_dist_desc.point_b
	$ClosestPoints/CylinderConvex/PointA.position  = cylinder_convex_dist_desc.point_a
	$ClosestPoints/CylinderConvex/PointB.position  = cylinder_convex_dist_desc.point_b
	$ClosestPoints/BoxConvex/PointA.position       = box_convex_dist_desc.point_a
	$ClosestPoints/BoxConvex/PointB.position       = box_convex_dist_desc.point_b
