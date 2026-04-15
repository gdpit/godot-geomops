@tool
extends Node3D

@onready
var collision_shapes: Array[CollisionShape3D] = [
    $Sphere/CollisionShape3D,
    $Capsule/CollisionShape3D,
    $Cylinder/CollisionShape3D,
    $Box/CollisionShape3D,
    $Convex/CollisionShape3D
]


func _init() -> void:
    DebugDraw3D.scoped_config()\
               .set_thickness(0.0)\
               .set_no_depth_test(true)

    GeomOps3D.set_tolerance(1e-4)
    GeomOps3D.set_max_iter(128)


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
    var params_a := GeomOpsParams3D.new()
    var params_b := GeomOpsParams3D.new()
    var result := GeomOpsResult3D.new()
    
    for i in collision_shapes.size() - 1:
        for j in range(i + 1, collision_shapes.size()):
            params_a.shape = collision_shapes[i].shape
            params_a.transform = collision_shapes[i].get_parent().global_transform
            
            params_b.shape = collision_shapes[j].shape
            params_b.transform = collision_shapes[j].get_parent().global_transform
            
            if GeomOps3D.closest_to_shape(params_a, params_b, result):
                DebugDraw3D.draw_sphere(result.point_a, 0.02, Color.GREEN)
                DebugDraw3D.draw_sphere(result.point_b, 0.02, Color.YELLOW)
                DebugDraw3D.draw_line(result.point_a, result.point_b, Color.DIM_GRAY)


    for collision_shape in collision_shapes:
        params_a.shape = collision_shape.shape
        params_a.transform = collision_shape.get_parent().global_transform   
        
        if GeomOps3D.closest_to_point(params_a, $Point.global_position, result):
            DebugDraw3D.draw_sphere(result.point_a, 0.02, Color.GREEN)
            DebugDraw3D.draw_sphere(result.point_b, 0.02, Color.YELLOW)
            DebugDraw3D.draw_line(result.point_a, result.point_b, Color.DIM_GRAY)
