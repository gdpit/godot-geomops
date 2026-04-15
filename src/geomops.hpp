#ifndef GEOMOPS_HPP
#define GEOMOPS_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace geomops {

class GeomOpsParams3D: public godot::RefCounted
{
    GDCLASS(GeomOpsParams3D, RefCounted)

    friend class GeomOps3D;

    godot::Transform3D transform;
    godot::Ref<godot::Resource> shape;

protected:
    static void _bind_methods();

public:    
    void set_transform(godot::Transform3D const & p_transform);
    godot::Transform3D const & get_transform() const;

    void set_shape(godot::Ref<godot::Resource> const & p_shape);
    godot::Ref<godot::Resource> get_shape() const;
};


class GeomOpsResult3D: public godot::RefCounted
{
    GDCLASS(GeomOpsResult3D, RefCounted)

    friend class GeomOps3D;

    godot::Vector3 point_a;
    godot::Vector3 point_b;

protected:
    static void _bind_methods();

public:    
    godot::Vector3 const & get_point_a() const;
    godot::Vector3 const & get_point_b() const;
};


class GeomOps3D: public godot::Object
{
    GDCLASS(GeomOps3D, Object)

    static GeomOps3D * instance;
    godot::real_t tolerance = 1e-3;
    size_t max_iter = 64;
    static godot::Vector3 support(GeomOpsParams3D const * const p_params, 
                          godot::Vector3 const & p_dir);
protected:
    static void _bind_methods();

public:
    static void create_singleton();
    static void delete_singleton();
    static GeomOps3D * get_singleton();

    godot::real_t get_tolerance();
    void set_tolerance(godot::real_t const t);
    size_t get_max_iter();
    void set_max_iter(size_t const n);
    bool closest_to_shape(godot::Ref<GeomOpsParams3D> p_params_a,
                          godot::Ref<GeomOpsParams3D> p_params_b,
                          godot::Ref<GeomOpsResult3D> r_result);
    bool closest_to_point(godot::Ref<GeomOpsParams3D> p_params,
                          godot::Vector3 p_point,
                          godot::Ref<GeomOpsResult3D> r_result);
};

}

#endif//GEOMOPS_HPP
