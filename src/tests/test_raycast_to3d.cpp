#include "test_raycast_to3d.h"
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#define MAKE_OBJECT_TYPE_HINT(m_type) vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, m_type)
void CTestRaycastTo3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_context"), &CTestRaycastTo3D::get_context);
    ClassDB::bind_method(D_METHOD("set_context", "context_node"), &CTestRaycastTo3D::set_context);

    ClassDB::bind_method(D_METHOD("get_hitting_is_true"), &CTestRaycastTo3D::get_hitting_is_true);
    ClassDB::bind_method(D_METHOD("set_hitting_is_true", "hitting"), &CTestRaycastTo3D::set_hitting_is_true);

    ClassDB::bind_method(D_METHOD("get_cast_from_context"), &CTestRaycastTo3D::get_cast_from_context);
    ClassDB::bind_method(D_METHOD("set_cast_from_context", "cast"), &CTestRaycastTo3D::set_cast_from_context);

    ClassDB::bind_method(D_METHOD("get_exclusions"), &CTestRaycastTo3D::get_exclusions);
    ClassDB::bind_method(D_METHOD("set_exclusions", "exclusion"), &CTestRaycastTo3D::set_exclusions);

    ClassDB::bind_method(D_METHOD("get_collision_mask"), &CTestRaycastTo3D::get_collision_mask);
    ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &CTestRaycastTo3D::set_collision_mask);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_NODE_TYPE, "CQueryContext3D"), "set_context", "get_context");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hitting_is_true"), "set_hitting_is_true", "get_hitting_is_true");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cast_from_context"), "set_cast_from_context", "get_cast_from_context");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exclusions", PROPERTY_HINT_ARRAY_TYPE, MAKE_OBJECT_TYPE_HINT("CollisionObject3D"), (PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_SCRIPT_VARIABLE)), "set_exclusions", "get_exclusions");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
}

CTestRaycastTo3D::CTestRaycastTo3D()
{
}

CTestRaycastTo3D::~CTestRaycastTo3D()
{
    context = nullptr;
}

void CTestRaycastTo3D::set_context(CQueryContext3D *context_node)
{
    context = context_node;
}

void CTestRaycastTo3D::set_hitting_is_true(bool hitting)
{
    hitting_is_true = hitting;
}

void CTestRaycastTo3D::set_cast_from_context(bool cast)
{
    cast_from_context = cast;
}

void CTestRaycastTo3D::set_exclusions(TypedArray<NodePath> exclusion)
{
    exclusions = exclusion;
}

void CTestRaycastTo3D::set_collision_mask(int mask)
{
    collision_mask = mask;
}

void CTestRaycastTo3D::perform_test(CQueryItem &projection)
{
    if (context == nullptr)
    {
        print_error("Test RaycastTo has no context");
    }
    Array context_nodes = context->get_context();
    // Make sure context only has 1 Node
    if (context_nodes.is_empty() || context_nodes.size() > 1)
    {
        print_error("RaycastTo context should have 1 value, and 1 value only.");
        return;
    }

    Node3D *context_node = Object::cast_to<Node3D>(context_nodes[0]);
    if (context_node == nullptr)
    {
        print_error("RaycastTo context should be Node3D");
        return;
    }
    PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
    Vector3 start_pos;
    Vector3 end_pos;

    if (cast_from_context)
    {
        start_pos = context_node->get_global_position();
        end_pos = projection.projection_position;
    }
    else
    {
        start_pos = projection.projection_position;
        end_pos = context_node->get_global_position();
    }

    Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(start_pos, end_pos);
    query->set_collision_mask(collision_mask);

    Array exclusion_rids = Array();

    for (NodePath exclusion : exclusions)
    {
        CollisionObject3D *node = Object::cast_to<CollisionObject3D>(get_node_or_null(exclusion));
        if (node == nullptr)
            continue;
        exclusion_rids.append(node->get_rid());
    }
    query->set_exclude(exclusion_rids);

    Dictionary result = space_state->intersect_ray(query);

    bool got_hit = false;

    Node3D *collider = Object::cast_to<Node3D>(result.get("collider", nullptr));
    if (collider == context_node)
        got_hit = true;

    bool filter;
    double score;

    if (got_hit)
    {
        if (hitting_is_true)
        {
            filter = false;
            score = 1.0;
        }
        else
        {
            filter = true;
            score = 0.0;
        }
    }
    else
    {
        if (hitting_is_true)
        {
            filter = true;
            score = 0.0;
        }
        else
        {
            filter = false;
            score = 1.0;
        }
    }

    switch (get_test_purpose())
    {
    case FILTER_SCORE:
    {
        projection.is_filtered = filter;
        if (!projection.is_filtered)
        {
            projection.add_score(score);
        }
        break;
    }
    case FILTER_ONLY:
        projection.is_filtered = filter;
        break;
    case SCORE_ONLY:
        projection.add_score(score);
    }
}

void CTestRaycastTo3D::_ready()
{
}
