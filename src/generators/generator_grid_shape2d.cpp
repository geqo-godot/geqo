#include "generators/generator_grid_shape2d.h"
#include "contexts/context_target_node2d.h"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/time.hpp"

void GeneratorGridShape2D::set_grid_half_size(double size) {
	grid_half_size = size;
}

void GeneratorGridShape2D::set_space_between(double space) {
	space_between = space;
}

void GeneratorGridShape2D::set_generate_around(QueryContext2D *context) {
	generate_around = context;
}

void GeneratorGridShape2D::perform_generation(Ref<QueryInstance2D> query_instance) {
	if (!generate_around)
		generate_around = Object::cast_to<QueryContext2D>(query_instance->get_querier_context());

	int grid_size = std::round(grid_half_size * 2 / space_between) + 1;
	//UtilityFunctions::print("The size of the grid: ", grid_size * grid_size);
	Array contexts = generate_around->get_context(query_instance);

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector2 starting_pos;
		if (contexts[context].get_type() == Variant::VECTOR2)
			starting_pos = contexts[context];
		else {
			Node2D *current_context = Object::cast_to<Node2D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node2D");
				continue;
			}
			starting_pos = current_context->get_global_position();
		}
		starting_pos.x -= grid_half_size;
		starting_pos.y -= grid_half_size;

		for (int y = _current_state.prev_y; y < grid_size; y++) {
			for (int x = _current_state.prev_x; x < grid_size; x++) {
				double pos_x = starting_pos.x + (x * space_between);
				double pos_y = starting_pos.y + (y * space_between);

				query_instance->add_item(QueryItem2D::create(Vector2(pos_x, pos_y)));

				// Check the time for stopping
				uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

				if (!query_instance->has_time_left()) {
					// Stop and wait until next frame
					_current_state.prev_context = context;
					_current_state.prev_x = x + 1;
					_current_state.prev_y = y;
					saved_instance = query_instance;
					get_tree()->connect("process_frame", callable_mp(this, &GeneratorGridShape2D::_on_next_process_frame), CONNECT_ONE_SHOT);
					return;
				}
			}
			// Start new row
			_current_state.prev_x = 0;
		}
		// Start over on new context
		_current_state.prev_y = 0;
	}
	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorGridShape2D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}

void GeneratorGridShape2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_half_size"), &GeneratorGridShape2D::get_grid_half_size);
	ClassDB::bind_method(D_METHOD("set_grid_half_size", "size"), &GeneratorGridShape2D::set_grid_half_size);

	ClassDB::bind_method(D_METHOD("get_space_between"), &GeneratorGridShape2D::get_space_between);
	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &GeneratorGridShape2D::set_space_between);

	ClassDB::bind_method(D_METHOD("get_generate_around"), &GeneratorGridShape2D::get_generate_around);
	ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &GeneratorGridShape2D::set_generate_around);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_generate_around", "get_generate_around");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "grid_half_size"), "set_grid_half_size", "get_grid_half_size");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "space_between"), "set_space_between", "get_space_between");

	ADD_GROUP("Projection Data", "");
}