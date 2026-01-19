#include "generators/query_generator2d.h"
#include "query_generator2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters2d.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void QueryGenerator2D::perform_tests(int current_item) {
	for (Variant test : get_children()) {
		QueryTest2D *current_test = Object::cast_to<QueryTest2D>(test);
		if (current_test == nullptr) {
			print_error("Invalid test, is this a QueryTest node?");
			continue;
		}
		QueryItem<Vector2> &item = get_query_items_ref()[current_item];
		if (!item.is_filtered)
			current_test->perform_test(item);
	}
}

Dictionary QueryGenerator2D::cast_ray_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, int col_mask) {
	PhysicsDirectSpaceState2D *space_state = get_world_2d()->get_direct_space_state();
	Ref<PhysicsRayQueryParameters2D> query = PhysicsRayQueryParameters2D::create(start_pos, end_pos, col_mask);

	if (get_raycast_mode() == AREA)
		query->set_collide_with_bodies(false);
	if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids = Array();

	for (Variant exclusion : exclusions) {
		CollisionObject2D *node = Object::cast_to<CollisionObject2D>(exclusion.operator Object *());
		if (node == nullptr)
			continue;
		exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);

	return space_state->intersect_ray(query);
}

void QueryGenerator2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_raycast_mode"), &QueryGenerator2D::get_raycast_mode);
	ClassDB::bind_method(D_METHOD("set_raycast_mode", "mode"), &QueryGenerator2D::set_raycast_mode);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");

	ADD_SIGNAL(MethodInfo("generator_finished"));
}
