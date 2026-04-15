#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>

#include "contexts/context_target_node3d.h"
#include "generator_in_array3d.h"

void GeneratorInArray3D::set_context_array(QueryContext3D *context) {
	context_array = context;
}

void GeneratorInArray3D::perform_generation(Ref<QueryInstance3D> query_instance) {
	if (!context_array)
		context_array = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());

	Array contexts = context_array->get_context(query_instance);

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		if (contexts[context].get_type() == Variant::VECTOR3) {
			Vector3 pos = contexts[context];
			query_instance->add_item(QueryItem3D::create(pos));
		} else {
			Node3D *current_context = Object::cast_to<Node3D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node3D or Vector3");
				continue;
			}
			query_instance->add_item(QueryItem3D::create(current_context->get_global_position(), current_context));
		}

		// Check the time for stopping
		uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

		if (!query_instance->has_time_left()) {
			// Stop and wait until next frame
			_current_state.prev_context = context;
			saved_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &GeneratorInArray3D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorInArray3D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}

void GeneratorInArray3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context_array", "context"), &GeneratorInArray3D::set_context_array);
	ClassDB::bind_method(D_METHOD("get_context_array"), &GeneratorInArray3D::get_context_array);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context_array", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_context_array", "get_context_array");
}
