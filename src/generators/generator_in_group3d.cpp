#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/world3d.hpp>

#include "generator_in_group3d.h"

void GeneratorInGroup3D::set_generate_around(QueryContext3D *context) {
	generate_around = context;
}

void GeneratorInGroup3D::set_group(String new_group) {
	group = new_group;
}

void GeneratorInGroup3D::set_range_radius(double new_range) {
	range_radius = new_range;
}

void GeneratorInGroup3D::set_collision_mask(int mask) {
	collision_mask = mask;
}

void GeneratorInGroup3D::perform_generation(uint64_t initial_time_usec, int time_budget_ms) {
	if (generate_around == nullptr) {
		print_error("Generator couldn't find Context");
		return;
	}
	Array contexts = generate_around->get_context();

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector3 pos;
		if (contexts[context].get_type() == Variant::VECTOR3)
			pos = contexts[context];
		else {
			Node3D *current_context = Object::cast_to<Node3D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node3D");
				continue;
			}
			pos = current_context->get_global_position();
		}

		// Cast sphere on each context

		PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
		Ref<PhysicsShapeQueryParameters3D> query = Ref<PhysicsShapeQueryParameters3D>();
		Ref<SphereShape3D> sphere = Ref<SphereShape3D>();
		query.instantiate();
		sphere.instantiate();

		sphere->set_radius(range_radius);
		query->set_shape(sphere);
		Transform3D transform = Transform3D();
		transform.set_origin(pos);
		query->set_transform(transform);

		if (get_raycast_mode() == AREA)
			query->set_collide_with_bodies(false);
		if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
			query->set_collide_with_areas(true);

		TypedArray<Dictionary> result = space_state->intersect_shape(query);

		// Check if they are Node3D before adding them to QueryItems
		for (Dictionary item : result) {
			Node3D *node = Object::cast_to<Node3D>(item.get("collider", nullptr));
			if (!node)
				continue;
			if (node->is_in_group(group)) {
				get_query_items_ref().push_back(QueryItem(node->get_global_position(), node));
				perform_tests(get_query_items_ref().size() - 1);
			} else
				continue;
		}

		// Check the time for stopping
		uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

		if (!has_time_left(initial_time_usec, current_time_usec, time_budget_ms)) {
			// UtilityFunctions::print("No time left, continue to next frame.");
			// Stop and wait until next frame
			_current_state.prev_context = context;
			_current_state.time_budget_ms = time_budget_ms;
			get_tree()->connect("process_frame", callable_mp(this, &GeneratorInGroup3D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorInGroup3D::_on_next_process_frame() {
	uint64_t initial_time_usec = Time::get_singleton()->get_ticks_usec();
	perform_generation(initial_time_usec, _current_state.time_budget_ms);
}

void GeneratorInGroup3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &GeneratorInGroup3D::set_generate_around);
	ClassDB::bind_method(D_METHOD("get_generate_around"), &GeneratorInGroup3D::get_generate_around);

	ClassDB::bind_method(D_METHOD("set_group", "new_group"), &GeneratorInGroup3D::set_group);
	ClassDB::bind_method(D_METHOD("get_group"), &GeneratorInGroup3D::get_group);

	ClassDB::bind_method(D_METHOD("set_range_radius", "new_radius"), &GeneratorInGroup3D::set_range_radius);
	ClassDB::bind_method(D_METHOD("get_range_radius"), &GeneratorInGroup3D::get_range_radius);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &GeneratorInGroup3D::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &GeneratorInGroup3D::get_collision_mask);

	ADD_GROUP("Generator", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_generate_around", "get_generate_around");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "group"), "set_group", "get_group");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_range_radius", "get_range_radius");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"collision_mask",
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_collision_mask",
			"get_collision_mask");
}
