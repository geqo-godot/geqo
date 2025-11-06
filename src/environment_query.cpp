#include "environment_query.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CEnvironmentQuery::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &CEnvironmentQuery::get_use_debug_shapes);
    ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &CEnvironmentQuery::set_use_debug_shapes);

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