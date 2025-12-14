#include "generators/generator_grid_shape3d.h"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/time.hpp"

void GeneratorGridShape3D::set_grid_half_size(double size) {
	grid_half_size = size;
}

void GeneratorGridShape3D::set_space_between(double space) {
	space_between = space;
}

void GeneratorGridShape3D::set_generate_around(QueryContext3D *context) {
	generate_around = context;
}

void GeneratorGridShape3D::set_use_vertical_projection(bool use) {
	use_vertical_projection = use;
}

void GeneratorGridShape3D::set_project_down(double project) {
	project_down = project;
}

void GeneratorGridShape3D::set_project_up(double project) {
	project_up = project;
}

void GeneratorGridShape3D::set_post_projection_vertical_offset(double offset) {
	post_projection_vertical_offset = offset;
}

void GeneratorGridShape3D::set_projection_collision_mask(int mask) {
	projection_collision_mask = mask;
}

void GeneratorGridShape3D::perform_generation(uint64_t initial_time_usec, int time_budget_ms) {
	if (generate_around == nullptr) {
		print_error("Generator couldn't find Context");
		return;
	}
	int grid_size = std::round(grid_half_size * 2 / space_between) + 1;
	//UtilityFunctions::print("The size of the grid: ", grid_size * grid_size);
	Array contexts = generate_around->get_context();

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector3 starting_pos;
		if (contexts[context].get_type() == Variant::VECTOR3)
			starting_pos = contexts[context];
		else {
			Node3D *current_context = Object::cast_to<Node3D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node3D");
				continue;
			}
			starting_pos = current_context->get_global_position();
		}
		starting_pos.x -= grid_half_size;
		starting_pos.z -= grid_half_size;

		for (int z = _current_state.prev_z; z < grid_size; z++) {
			for (int x = _current_state.prev_x; x < grid_size; x++) {
				double pos_x = starting_pos.x + (x * space_between);
				double pos_z = starting_pos.z + (z * space_between);

				if (use_vertical_projection) {
					Vector3 ray_pos = Vector3(pos_x, starting_pos.y, pos_z);
					Dictionary ray_result = cast_ray_projection(
							ray_pos + (Vector3(0, project_up, 0)),
							ray_pos + (Vector3(0, -project_down, 0)), contexts, projection_collision_mask);

					if (!ray_result.is_empty()) {
						Vector3 casted_position = (Vector3)ray_result.get("position", Vector3());
						Node3D *collider = Object::cast_to<Node3D>(ray_result.get("collider", nullptr));
						get_query_items_ref().push_back(
								QueryItem(casted_position + Vector3(0, post_projection_vertical_offset, 0), collider));
						perform_tests(get_query_items_ref().size() - 1);
					}
				} else {
					get_query_items_ref().push_back(QueryItem(Vector3(pos_x, starting_pos.y, pos_z)));
					perform_tests(get_query_items_ref().size() - 1);
				}

				// Check the time for stopping
				uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

				if (!has_time_left(initial_time_usec, current_time_usec, time_budget_ms)) {
					// UtilityFunctions::print("No time left, continue to next frame.");
					// Stop and wait until next frame
					_current_state.prev_context = context;
					_current_state.prev_x = x + 1;
					_current_state.prev_z = z;
					_current_state.time_budget_ms = time_budget_ms;
					get_tree()->connect("process_frame", callable_mp(this, &GeneratorGridShape3D::_on_next_process_frame), CONNECT_ONE_SHOT);
					return;
				}
			}
			// Start new row
			_current_state.prev_x = 0;
		}
		// Start over on new context
		_current_state.prev_z = 0;
	}
	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorGridShape3D::_on_next_process_frame() {
	// UtilityFunctions::print("Next process frame called.");
	uint64_t initial_time_usec = Time::get_singleton()->get_ticks_usec();
	perform_generation(initial_time_usec, _current_state.time_budget_ms);
}

void GeneratorGridShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_half_size"), &GeneratorGridShape3D::get_grid_half_size);
	ClassDB::bind_method(D_METHOD("set_grid_half_size", "size"), &GeneratorGridShape3D::set_grid_half_size);

	ClassDB::bind_method(D_METHOD("get_space_between"), &GeneratorGridShape3D::get_space_between);
	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &GeneratorGridShape3D::set_space_between);

	ClassDB::bind_method(D_METHOD("get_generate_around"), &GeneratorGridShape3D::get_generate_around);
	ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &GeneratorGridShape3D::set_generate_around);

	ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &GeneratorGridShape3D::get_use_vertical_projection);
	ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &GeneratorGridShape3D::set_use_vertical_projection);

	ClassDB::bind_method(D_METHOD("get_project_down"), &GeneratorGridShape3D::get_project_down);
	ClassDB::bind_method(D_METHOD("set_project_down", "project"), &GeneratorGridShape3D::set_project_down);

	ClassDB::bind_method(D_METHOD("get_project_up"), &GeneratorGridShape3D::get_project_up);
	ClassDB::bind_method(D_METHOD("set_project_up", "project"), &GeneratorGridShape3D::set_project_up);

	ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &GeneratorGridShape3D::get_post_projection_vertical_offset);
	ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &GeneratorGridShape3D::set_post_projection_vertical_offset);

	ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &GeneratorGridShape3D::get_projection_collision_mask);
	ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &GeneratorGridShape3D::set_projection_collision_mask);

	ADD_GROUP("Generator", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_generate_around", "get_generate_around");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "grid_half_size"), "set_grid_half_size", "get_grid_half_size");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "space_between"), "set_space_between", "get_space_between");

	ADD_GROUP("Projection Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_vertical_projection"), "set_use_vertical_projection", "get_use_vertical_projection");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "project_up"), "set_project_up", "get_project_up");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "project_down"), "set_project_down", "get_project_down");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "post_projection_vertical_offset"), "set_post_projection_vertical_offset", "get_post_projection_vertical_offset");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"projection_collision_mask",
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_projection_collision_mask",
			"get_projection_collision_mask");
}