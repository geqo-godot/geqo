#include "query_generator3d.h"
#include "query_test3d.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CQueryGenerator3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_raycast_mode"), &CQueryGenerator3D::get_raycast_mode);
    ClassDB::bind_method(D_METHOD("set_raycast_mode", "mode"), &CQueryGenerator3D::set_raycast_mode);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");
}

CQueryGenerator3D::CQueryGenerator3D()
{
    // Initialize any variables here.
}

CQueryGenerator3D::~CQueryGenerator3D()
{
    // Add your cleanup here.
}

CQueryGenerator3D::RaycastMode CQueryGenerator3D::get_raycast_mode() const
{
    return raycast_mode;
}

void CQueryGenerator3D::set_raycast_mode(RaycastMode mode)
{
    raycast_mode = mode;
}

void CQueryGenerator3D::perform_tests(vector<CQueryItem> &query_item_list)
{
    for (Variant test : get_children())
    {
        // TODO: Make sure generator only has QueryTest children
        for (CQueryItem &query_item : query_item_list)
        {
            CQueryTest3D *current_test = Object::cast_to<CQueryTest3D>(test);
            if (current_test == nullptr)
            {
                print_error("Invalid test, is this a QueryTest node?");
                continue;
            }
            current_test->perform_test(query_item);
        }
    }
}
