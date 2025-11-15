#include "generator_grid_shape3d.h"
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

void CGeneratorGridShape3D::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_grid_half_size"), &CGeneratorGridShape3D::get_grid_half_size);
    ClassDB::bind_method(D_METHOD("set_grid_half_size", "size"), &CGeneratorGridShape3D::set_grid_half_size);

    ClassDB::bind_method(D_METHOD("get_space_between"), &CGeneratorGridShape3D::get_space_between);
    ClassDB::bind_method(D_METHOD("set_space_between", "space"), &CGeneratorGridShape3D::set_space_between);

    ClassDB::bind_method(D_METHOD("get_generate_around"), &CGeneratorGridShape3D::get_generate_around);
    ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &CGeneratorGridShape3D::set_generate_around);

    ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &CGeneratorGridShape3D::get_use_vertical_projection);
    ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &CGeneratorGridShape3D::set_use_vertical_projection);

    ClassDB::bind_method(D_METHOD("get_project_down"), &CGeneratorGridShape3D::get_project_down);
    ClassDB::bind_method(D_METHOD("set_project_down", "project"), &CGeneratorGridShape3D::set_project_down);

    ClassDB::bind_method(D_METHOD("get_project_up"), &CGeneratorGridShape3D::get_project_up);
    ClassDB::bind_method(D_METHOD("set_project_up", "project"), &CGeneratorGridShape3D::set_project_up);

    ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &CGeneratorGridShape3D::get_post_projection_vertical_offset);
    ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &CGeneratorGridShape3D::set_post_projection_vertical_offset);

    ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &CGeneratorGridShape3D::get_projection_collision_mask);
    ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &CGeneratorGridShape3D::set_projection_collision_mask);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "CQueryContext3D"), "set_generate_around", "get_generate_around");
}

CGeneratorGridShape3D::CGeneratorGridShape3D()
{
}

CGeneratorGridShape3D::~CGeneratorGridShape3D()
{
}

void godot::CGeneratorGridShape3D::_ready()
{
}

double CGeneratorGridShape3D::get_grid_half_size() const
{
    return grid_half_size;
}

void CGeneratorGridShape3D::set_grid_half_size(double size)
{
    grid_half_size = size;
}

double CGeneratorGridShape3D::get_space_between() const
{
    return space_between;
}

void CGeneratorGridShape3D::set_space_between(double space)
{
    space_between = space;
}

CQueryContext3D *CGeneratorGridShape3D::get_generate_around()
{
    return generate_around;
}

void CGeneratorGridShape3D::set_generate_around(CQueryContext3D *context)
{
    generate_around = context;
}

bool CGeneratorGridShape3D::get_use_vertical_projection() const
{
    return use_vertical_projection;
}

void CGeneratorGridShape3D::set_use_vertical_projection(bool use)
{
    use_vertical_projection = use;
}

double CGeneratorGridShape3D::get_project_down() const
{
    return project_down;
}

void CGeneratorGridShape3D::set_project_down(double project)
{
    project_down = project;
}

double CGeneratorGridShape3D::get_project_up() const
{
    return project_up;
}

void CGeneratorGridShape3D::set_project_up(double project)
{
    project_up = project;
}

double CGeneratorGridShape3D::get_post_projection_vertical_offset() const
{
    return post_projection_vertical_offset;
}

void CGeneratorGridShape3D::set_post_projection_vertical_offset(double offset)
{
    post_projection_vertical_offset = offset;
}

int CGeneratorGridShape3D::get_projection_collision_mask() const
{
    return projection_collision_mask;
}

void CGeneratorGridShape3D::set_projection_collision_mask(int mask)
{
    projection_collision_mask = mask;
}

void CGeneratorGridShape3D::perform_generation(vector<CQueryItem> query_item_list)
{
    if (generate_around == nullptr)
    {
        print_error("Generator couldn't find Context");
        return;
    }
    int grid_size = std::round(grid_half_size * 2 / space_between) + 1;
    Array contexts = generate_around->get_context();

    for (Variant context : contexts)
    {
        Vector3 starting_pos;
        if (context.get_type() == Variant::VECTOR3)
            starting_pos = context;
        else
        {
            Node3D *current_context = Object::cast_to<Node3D>(context);
            if (current_context == nullptr)
            {
                print_error("Context is invalid, must be Node3D");
                continue;
            }
            starting_pos = current_context->get_global_position();
        }
        starting_pos.x -= grid_half_size;
        starting_pos.z -= grid_half_size;

        for (int z = 0; z < grid_size; z++)
        {
            for (int x = 0; x < grid_size; x++)
            {
                double pos_x = starting_pos.x + (x * space_between);
                double pos_z = starting_pos.z + (z * space_between);

                if (!use_vertical_projection)
                {
                    query_item_list.push_back(CQueryItem(Vector3(pos_x, starting_pos.y, pos_z)));
                    continue;
                }

                Vector3 ray_pos = Vector3(pos_x, starting_pos.y, pos_z);
                Dictionary ray_result = cast_ray_projection(
                    ray_pos + (Vector3(0, 1, 0) * project_up),
                    ray_pos + (Vector3(0, -1, 0) * project_down), contexts);

                if (!ray_result.is_empty())
                {

                    Vector3 casted_position = (Vector3)ray_result.get("position", Vector3());
                    // TODO: Get the collider also
                    query_item_list.push_back(
                        CQueryItem(casted_position + Vector3(0, 1, 0) * post_projection_vertical_offset));
                }
            }
        }
    }
}

Dictionary CGeneratorGridShape3D::cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions)
{
    PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
    Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(start_pos, end_pos, projection_collision_mask);

    if (get_raycast_mode() == AREA)
        query->set_collide_with_bodies(false);
    if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
        query->set_collide_with_areas(true);

    Array exclusion_rids = Array();

    // TODO: Figure out how to get the RIDs
    for (Variant exclusion : exclusions)
    {
        Node *node = Object::cast_to<Node>(exclusion.operator Object *());
        if (node == nullptr)
            continue;
        // exclusion_rids.append(node->get_instance_id());
    }
    // query->set_exclude(exclusion_rids);

    return space_state->intersect_ray(query);
}
