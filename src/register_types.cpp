#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "query_context3d.h"
#include "query_generator3d.h"
#include "query_result.h"
#include "query_test3d.h"

#include "environment_query.h"
#include "geqo_debug_spheres.h"
// Contexts
#include "context_in_group3d.h"
#include "context_target_node3d.h"

// Generators
#include "generator_circle_shape3d.h"
#include "generator_grid_shape3d.h"

// Tests
#include "test_distance_to3d.h"
#include "test_dot_product3d.h"
#include "test_raycast_to3d.h"

#include "register_types.h"

using namespace godot;

void initialize_geqo_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<GEQODebugSpheres>();
	ClassDB::register_class<QueryResult>();
	ClassDB::register_class<EnvironmentQuery>();
	ClassDB::register_abstract_class<QueryContext3D>();
	ClassDB::register_abstract_class<QueryGenerator3D>();
	ClassDB::register_abstract_class<QueryTest3D>();

	// Generators
	ClassDB::register_class<GeneratorGridShape3D>();
	ClassDB::register_class<GeneratorCircleShape3D>();

	// Contexts
	ClassDB::register_class<ContextTargetNode3D>();
	ClassDB::register_class<ContextInGroup3D>();

	// Tests
	ClassDB::register_class<TestDistanceTo3D>();
	ClassDB::register_class<TestRaycastTo3D>();
	ClassDB::register_class<TestDotProduct3D>();
}

void uninitialize_geqo_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT geqo_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_geqo_module);
	init_obj.register_terminator(uninitialize_geqo_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}