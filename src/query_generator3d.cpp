#include "query_generator3d.h"
#include "query_test3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CQueryGenerator3D::set_raycast_mode(RaycastMode mode) {
	raycast_mode = mode;
}

void CQueryGenerator3D::perform_tests(std::vector<CQueryItem> &query_item_list) {
	for (Variant test : get_children()) {
		CQueryTest3D *current_test = Object::cast_to<CQueryTest3D>(test);
		if (current_test == nullptr) {
			print_error("Invalid test, is this a QueryTest node?");
			continue;
		}
		for (CQueryItem &query_item : query_item_list)
			current_test->perform_test(query_item);
	}
}

Dictionary CQueryGenerator3D::cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask) {
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
	Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(start_pos, end_pos, col_mask);

	if (get_raycast_mode() == AREA)
		query->set_collide_with_bodies(false);
	if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids = Array();

	for (Variant exclusion : exclusions) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(exclusion.operator Object *());
		if (node == nullptr)
			continue;
		exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);

	return space_state->intersect_ray(query);
}

void CQueryGenerator3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_raycast_mode"), &CQueryGenerator3D::get_raycast_mode);
	ClassDB::bind_method(D_METHOD("set_raycast_mode", "mode"), &CQueryGenerator3D::set_raycast_mode);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");
}