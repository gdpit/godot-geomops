#include "register_types.hpp"
#include "distutils.hpp"


using namespace godot;


void initialize_distutils_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(DistanceDescriptor);
	GDREGISTER_CLASS(DistUtils);
}


void uninitialize_distutils_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}


extern "C" {
    GDExtensionBool GDE_EXPORT distutils_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_distutils_module);
        init_obj.register_terminator(uninitialize_distutils_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}