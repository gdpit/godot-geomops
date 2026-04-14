@tool
extends Node3D


func _init() -> void:
    DebugDraw3D.scoped_config()\
               .set_thickness(0.0)\
               .set_no_depth_test(true)

    GeomOps3D.set_tolerance(1e-4)
    GeomOps3D.set_max_iter(8)


func _ready() -> void:    
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

    var distance_descriptors: Array[GeomOpsClosestPointPair3D] = [
        GeomOps3D.closest_shape($Sphere/CollisionShape3D, $Capsule/CollisionShape3D),
        GeomOps3D.closest_shape($Sphere/CollisionShape3D, $Cylinder/CollisionShape3D),
        GeomOps3D.closest_shape($Sphere/CollisionShape3D, $Box/CollisionShape3D),
        GeomOps3D.closest_shape($Sphere/CollisionShape3D, $Convex/CollisionShape3D),
        GeomOps3D.closest_shape($Capsule/CollisionShape3D, $Cylinder/CollisionShape3D),
        GeomOps3D.closest_shape($Capsule/CollisionShape3D, $Box/CollisionShape3D),
        GeomOps3D.closest_shape($Capsule/CollisionShape3D, $Convex/CollisionShape3D),
        GeomOps3D.closest_shape($Cylinder/CollisionShape3D, $Box/CollisionShape3D),
        GeomOps3D.closest_shape($Cylinder/CollisionShape3D, $Convex/CollisionShape3D),
        GeomOps3D.closest_shape($Box/CollisionShape3D, $Convex/CollisionShape3D),
        
        GeomOps3D.closest_point($Sphere/CollisionShape3D, $Point.position),
        GeomOps3D.closest_point($Capsule/CollisionShape3D, $Point.position),
        GeomOps3D.closest_point($Cylinder/CollisionShape3D, $Point.position),
        GeomOps3D.closest_point($Box/CollisionShape3D, $Point.position),
        GeomOps3D.closest_point($Convex/CollisionShape3D, $Point.position)
    ]
    
    for distance_descriptor in distance_descriptors:
        DebugDraw3D.draw_sphere(distance_descriptor.point_a, 0.02, Color.GREEN)
        DebugDraw3D.draw_sphere(distance_descriptor.point_b, 0.02, Color.YELLOW)
        DebugDraw3D.draw_line(distance_descriptor.point_a, distance_descriptor.point_b, Color.DIM_GRAY)
        
