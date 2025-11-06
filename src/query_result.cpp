#include "query_result.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CQueryResult::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_highest_score_position"), &CQueryResult::get_highest_score_position);
    ClassDB::bind_method(D_METHOD("get_highest_score_node"), &CQueryResult::get_highest_score_node);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
}

CQueryResult::CQueryResult()
{
    // Initialize any variables here.
}

CQueryResult::~CQueryResult()
{
    // Add your cleanup here.
}

Vector3 CQueryResult::get_highest_score_position() const
{
    return Vector3();
}

Node *CQueryResult::get_highest_score_node() const
{
    Node *node;
    return node;
}
