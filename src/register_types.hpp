#ifndef DISTUTILS_REGISTER_TYPES_H
#define DISTUTILS_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_distutils_module(ModuleInitializationLevel p_level);
void uninitialize_distutils_module(ModuleInitializationLevel p_level);

#endif//DISTUTILS_REGISTER_TYPES_H