#include "environment_query.h"
#include "query_generator3d.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void CEnvironmentQuery::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &CEnvironmentQuery::get_use_debug_shapes);
    ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &CEnvironmentQuery::set_use_debug_shapes);

    ClassDB::bind_method(D_METHOD("request_query"), &CEnvironmentQuery::request_query);

    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
}

CEnvironmentQuery::CEnvironmentQuery()
{
    // Initialize any variables here.
    use_debug_shapes = false;
}

CEnvironmentQuery::~CEnvironmentQuery()
{
    // Add your cleanup here.
}

void CEnvironmentQuery::set_use_debug_shapes(const bool use_debug)
{
    use_debug_shapes = use_debug;
}

bool CEnvironmentQuery::get_use_debug_shapes() const
{
    return use_debug_shapes;
}

void godot::CEnvironmentQuery::_ready()
{
    if (Engine::get_singleton()->is_editor_hint())
        return;
    debug_spheres = memnew(CGEQODebugSpheres);
    call_deferred("add_sibling", debug_spheres);
}

Ref<CQueryResult> CEnvironmentQuery::request_query()
{
    UtilityFunctions::print_rich(String("Requested a query in C++"));
    query_items.clear();
    UtilityFunctions::print_rich(get_child_count());

    for (Variant child : get_children())
    {
        CQueryGenerator3D *generator = cast_to<CQueryGenerator3D>(child);
        if (generator == nullptr)
        {
            print_error("EnvironmentQuery: Child is not Generator");
            continue;
        }
        generator->perform_generation(query_items);
        generator->perform_tests(query_items);
    }

    // TODO: Reimplement draw debug

    Ref<CQueryResult> result;
    result.instantiate();
    result->set_items(query_items);

    if (use_debug_shapes)
    {
        debug_spheres->draw_items(query_items);
    }
    UtilityFunctions::print("Results: ", result->get_highest_score_position());
    UtilityFunctions::print("Amount of items: ", result->get_query_items().size());
    return result;
}
