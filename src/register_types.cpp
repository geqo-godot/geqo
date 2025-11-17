#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "query_result.h"
#include "environment_query.h"
#include "query_context3d.h"
#include "query_generator3d.h"
#include "query_test3d.h"

#include <debug/geqo_debug_spheres.h>
// Contexts
#include <contexts/context_target_node3d.h>
#include <contexts/context_in_group3d.h>

// Generators
#include <generators/generator_circle_shape3d.h>
#include <generators/generator_grid_shape3d.h>

// Tests
#include <tests/test_distance_to3d.h>
#include <tests/test_raycast_to3d.h>
#include <tests/test_dot_product3d.h>

#include "register_types.h"

using namespace godot;

void initialize_geqo_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }

    ClassDB::register_class<CGEQODebugSpheres>();
    ClassDB::register_class<CQueryResult>();
    ClassDB::register_class<CEnvironmentQuery>();
    ClassDB::register_abstract_class<CQueryContext3D>();
    ClassDB::register_abstract_class<CQueryGenerator3D>();
    ClassDB::register_abstract_class<CQueryTest3D>();

    // Contexts
    ClassDB::register_class<CContextTargetNode3D>();
    ClassDB::register_class<CContextInGroup3D>();

    // Generators
    ClassDB::register_class<CGeneratorGridShape3D>();
    ClassDB::register_class<CGeneratorCircleShape3D>();

    // Tests
    ClassDB::register_class<CTestDistanceTo3D>();
    ClassDB::register_class<CTestRaycastTo3D>();
    ClassDB::register_class<CTestDotProduct3D>();
}

void uninitialize_geqo_module(ModuleInitializationLevel p_level)
{
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }
}

extern "C"
{
    // Initialization.
    GDExtensionBool GDE_EXPORT geqo_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_geqo_module);
        init_obj.register_terminator(uninitialize_geqo_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}