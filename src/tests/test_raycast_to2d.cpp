#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters2d.hpp>
#include <godot_cpp/classes/world2d.hpp>

#include "tests/test_raycast_to2d.h"
#define MAKE_OBJECT_TYPE_HINT(m_type) vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, m_type)

void TestRaycastTo2D::set_context(QueryContext2D *context_node) {
	context = context_node;
}

void TestRaycastTo2D::set_hitting_is_true(bool hitting) {
	hitting_is_true = hitting;
}

void TestRaycastTo2D::set_cast_from_context(bool cast) {
	cast_from_context = cast;
}

void TestRaycastTo2D::set_exclusions(TypedArray<NodePath> exclusion) {
	exclusions = exclusion;
}

void TestRaycastTo2D::set_collision_mask(int mask) {
	collision_mask = mask;
}

void TestRaycastTo2D::perform_test(QueryItem<Vector2> &projection) {
	if (!context) {
		print_error("Test RaycastTo has no context");
		return;
	}
	Array context_nodes = context->get_context();

	int current_score = 0;

	for (Variant context : context_nodes) {
		Node2D *context_node = Object::cast_to<Node2D>(context);
		if (context_node == nullptr) {
			print_error("RaycastTo context should be PhysicsBody2D");
			return;
		}
		PhysicsDirectSpaceState2D *space_state = get_world_2d()->get_direct_space_state();
		Vector2 start_pos;
		Vector2 end_pos;

		if (cast_from_context) {
			start_pos = context_node->get_global_position();
			end_pos = projection.projection_position;
		} else {
			start_pos = projection.projection_position;
			end_pos = context_node->get_global_position();
		}

		Ref<PhysicsRayQueryParameters2D> query = PhysicsRayQueryParameters2D::create(start_pos, end_pos);
		query->set_collision_mask(collision_mask);

		Array exclusion_rids = Array();

		for (NodePath exclusion : exclusions) {
			CollisionObject2D *node = Object::cast_to<CollisionObject2D>(get_node_or_null(exclusion));
			if (node == nullptr)
				continue;
			exclusion_rids.append(node->get_rid());
		}
		query->set_exclude(exclusion_rids);

		Dictionary result = space_state->intersect_ray(query);

		bool is_hit = false;

		Node2D *collider = Object::cast_to<Node2D>(result.get("collider", nullptr));
		if (collider == context_node)
			is_hit = true;

		// Invert is_hit if hitting_is_true is false
		if (!hitting_is_true) {
			is_hit = !is_hit;
		}

		if (is_hit) {
			current_score++;
			if (get_context_filter_operator() == ANY_PASS)
				break;
		}
	}
	bool filter = false;
	int final_score = 0;
	FilterOperator filter_op = get_context_filter_operator();

	switch (filter_op) {
		case ANY_PASS: {
			if (current_score > 0) {
				final_score = 1;
			} else {
				filter = true;
			}
		} break;
		case ALL_PASS: {
			if (current_score == context_nodes.size()) {
				final_score = 1;
			} else {
				filter = true;
			}
		} break;
	}

	switch (get_context_score_operator()) {
		case FILTER_SCORE: {
			projection.is_filtered = filter;
			projection.add_score(final_score);
		} break;
		case FILTER_ONLY: {
			projection.is_filtered = filter;
		} break;
		case SCORE_ONLY: {
			projection.add_score(final_score);
		} break;
	}
}

void TestRaycastTo2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_context"), &TestRaycastTo2D::get_context);
	ClassDB::bind_method(D_METHOD("set_context", "context_node"), &TestRaycastTo2D::set_context);

	ClassDB::bind_method(D_METHOD("get_hitting_is_true"), &TestRaycastTo2D::get_hitting_is_true);
	ClassDB::bind_method(D_METHOD("set_hitting_is_true", "hitting"), &TestRaycastTo2D::set_hitting_is_true);

	ClassDB::bind_method(D_METHOD("get_cast_from_context"), &TestRaycastTo2D::get_cast_from_context);
	ClassDB::bind_method(D_METHOD("set_cast_from_context", "cast"), &TestRaycastTo2D::set_cast_from_context);

	ClassDB::bind_method(D_METHOD("get_exclusions"), &TestRaycastTo2D::get_exclusions);
	ClassDB::bind_method(D_METHOD("set_exclusions", "exclusion"), &TestRaycastTo2D::set_exclusions);

	ClassDB::bind_method(D_METHOD("get_collision_mask"), &TestRaycastTo2D::get_collision_mask);
	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &TestRaycastTo2D::set_collision_mask);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_context", "get_context");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hitting_is_true"), "set_hitting_is_true", "get_hitting_is_true");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cast_from_context"), "set_cast_from_context", "get_cast_from_context");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_filter_operator", PROPERTY_HINT_ENUM, "Any Pass,All Pass"), "set_context_filter_operator", "get_context_filter_operator");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exclusions", PROPERTY_HINT_ARRAY_TYPE, MAKE_OBJECT_TYPE_HINT("CollisionObject2D"), (PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_SCRIPT_VARIABLE)), "set_exclusions", "get_exclusions");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
}