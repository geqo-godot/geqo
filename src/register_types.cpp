#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "query_enums.h"

#include "query_instance.h"
#include "query_item.h"

#include "contexts/query_context2d.h"
#include "contexts/query_context3d.h"

#include "generators/query_generator2d.h"
#include "generators/query_generator3d.h"

#include "query_result.h"

#include "tests/query_test2d.h"
#include "tests/query_test3d.h"

#include "debug/geqo_debug.h"
#include "debug/geqo_debug_spheres2d.h"
#include "debug/geqo_debug_spheres3d.h"

#include "environment_query2d.h"
#include "environment_query3d.h"
// Contexts
#include "contexts/context_in_group2d.h"
#include "contexts/context_in_group3d.h"

#include "contexts/context_target_array2d.h"
#include "contexts/context_target_array3d.h"

#include "contexts/context_target_node2d.h"
#include "contexts/context_target_node3d.h"

#include "contexts/context_query_items2d.h"
#include "contexts/context_query_items3d.h"

// Generators
#include "generators/generator_circle_shape2d.h"
#include "generators/generator_circle_shape3d.h"
#include "generators/generator_grid_shape2d.h"
#include "generators/generator_grid_shape3d.h"
#include "generators/generator_in_array3d.h"
#include "generators/generator_in_group2d.h"
#include "generators/generator_in_group3d.h"

// Tests
#include "tests/test_distance_to2d.h"
#include "tests/test_distance_to3d.h"

#include "tests/test_dot_product2d.h"
#include "tests/test_dot_product3d.h"

#include "tests/test_raycast_to2d.h"
#include "tests/test_raycast_to3d.h"

#include "tests/test_path_find_to3d.h"

#include "tests/test_intersects_area3d.h"

#include "register_types.h"

using namespace godot;

static GEQODebug *_geqo_debug_singleton;

void initialize_geqo_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Debugging
	ClassDB::register_abstract_class<GEQOEnums>();
	ClassDB::register_class<GEQODebug>();
	_geqo_debug_singleton = memnew(GEQODebug);
	Engine::get_singleton()->register_singleton("GEQODebug", GEQODebug::get_singleton());
	_geqo_debug_singleton->init();
	ClassDB::register_class<GEQODebugDraw2D>();
	ClassDB::register_class<GEQODebugSpheres2D>();
	ClassDB::register_class<GEQODebugSpheres3D>();

	ClassDB::register_class<QueryItem2D>();
	ClassDB::register_class<QueryItem3D>();
	ClassDB::register_class<QueryResult2D>();
	ClassDB::register_class<QueryResult3D>();

	ClassDB::register_class<QueryInstance2D>();
	ClassDB::register_class<QueryInstance3D>();
	ClassDB::register_class<EnvironmentQuery2D>();
	ClassDB::register_class<EnvironmentQuery3D>();

	ClassDB::register_class<QueryContext2D>();
	ClassDB::register_class<QueryContext3D>();

	ClassDB::register_abstract_class<QueryGenerator2D>();
	ClassDB::register_abstract_class<QueryGenerator3D>();

	ClassDB::register_class<QueryTest2D>();
	ClassDB::register_class<QueryTest3D>();

	// Generators
	ClassDB::register_class<GeneratorGridShape2D>();
	ClassDB::register_class<GeneratorGridShape3D>();

	ClassDB::register_class<GeneratorCircleShape2D>();
	ClassDB::register_class<GeneratorCircleShape3D>();

	ClassDB::register_class<GeneratorInGroup2D>();
	ClassDB::register_class<GeneratorInGroup3D>();

	ClassDB::register_class<GeneratorInArray3D>();

	// Contexts
	ClassDB::register_class<ContextTargetNode2D>();
	ClassDB::register_class<ContextTargetNode3D>();
	ClassDB::register_class<ContextTargetArray2D>();
	ClassDB::register_class<ContextTargetArray3D>();
	ClassDB::register_class<ContextInGroup2D>();
	ClassDB::register_class<ContextInGroup3D>();
	ClassDB::register_class<ContextQueryItems2D>();
	ClassDB::register_class<ContextQueryItems3D>();

	// Tests
	ClassDB::register_class<TestDistanceTo2D>();
	ClassDB::register_class<TestDistanceTo3D>();

	ClassDB::register_class<TestRaycastTo2D>();
	ClassDB::register_class<TestRaycastTo3D>();
	ClassDB::register_class<TestDotProduct2D>();
	ClassDB::register_class<TestDotProduct3D>();
	ClassDB::register_class<TestPathFindTo3D>();
	ClassDB::register_class<TestIntersectsArea3D>();
}

void uninitialize_geqo_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Engine::get_singleton()->unregister_singleton("GEQODebug");
	memdelete(_geqo_debug_singleton);
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