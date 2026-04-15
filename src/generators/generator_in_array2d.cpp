#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>

#include "contexts/context_target_node2d.h"
#include "generator_in_array2d.h"

void GeneratorInArray2D::set_context_array(QueryContext2D *context) {
	context_array = context;
}

void GeneratorInArray2D::perform_generation(Ref<QueryInstance2D> query_instance) {
	if (!context_array)
		context_array = Object::cast_to<QueryContext2D>(query_instance->get_querier_context());

	Array contexts = context_array->get_context(query_instance);

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		if (contexts[context].get_type() == Variant::VECTOR2) {
			Vector2 pos = contexts[context];
			query_instance->add_item(QueryItem2D::create(pos));
		} else {
			Node2D *current_context = Object::cast_to<Node2D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node2D or Vector2");
				continue;
			}
			query_instance->add_item(QueryItem2D::create(current_context->get_global_position(), current_context));
		}

		// Check the time for stopping
		uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

		if (!query_instance->has_time_left()) {
			// Stop and wait until next frame
			_current_state.prev_context = context;
			saved_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &GeneratorInArray2D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorInArray2D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}

void GeneratorInArray2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context_array", "context"), &GeneratorInArray2D::set_context_array);
	ClassDB::bind_method(D_METHOD("get_context_array"), &GeneratorInArray2D::get_context_array);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context_array", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_context_array", "get_context_array");
}
