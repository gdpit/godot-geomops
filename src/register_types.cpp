#include "geomops.hpp"
#include "register_types.hpp"
#include "godot_cpp/classes/engine.hpp"

void initialize_geomops_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(geomops::GeomOpsParams3D);
	GDREGISTER_CLASS(geomops::GeomOpsResult3D);
	GDREGISTER_CLASS(geomops::GeomOps3D);

	geomops::GeomOps3D::create_singleton();
	godot::Engine::get_singleton()->register_singleton("GeomOps3D", geomops::GeomOps3D::get_singleton());
}


void uninitialize_geomops_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::Engine::get_singleton()->unregister_singleton("GeomOps3D");
	geomops::GeomOps3D::delete_singleton();
}


extern "C" {
    GDExtensionBool GDE_EXPORT geomops_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, 
													const GDExtensionClassLibraryPtr p_library, 
													GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_geomops_module);
        init_obj.register_terminator(uninitialize_geomops_module);
        init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}