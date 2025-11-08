#include "environment_query.h"
#include "query_generator3d.h"
#include <godot_cpp/core/class_db.hpp>

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

Ref<CQueryResult> CEnvironmentQuery::request_query()
{
    UtilityFunctions::print_rich(String("Requested a query in C++"));
    query_items.clear();
    UtilityFunctions::print_rich(get_child_count());

    for (Variant child : get_children())
    {
        // TODO: Verify child is generator
        CQueryGenerator3D *generator = cast_to<CQueryGenerator3D>(child);
        generator->perform_generation(query_items);
        generator->perform_tests(query_items);
    }

    // TODO: Reimplement draw debug

    Ref<CQueryResult> result;
    result.instantiate();
    result->set_items(query_items);

    return result;
}
