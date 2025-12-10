#include "environment_query.h"
#include "generators/query_generator3d.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

//void EnvironmentQuery::set_use_debug_shapes(const bool use_debug) {
//	use_debug_shapes = use_debug;
//}
void EnvironmentQuery::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			if (Engine::get_singleton()->is_editor_hint())
				return;
			if (!is_inside_tree())
				return;
			if (get_tree()->is_debugging_navigation_hint()) {
				debug_spheres = memnew(GEQODebugSpheres);
				call_deferred("add_sibling", debug_spheres);
			}
			init_generators();
		} break;
	}
}

void EnvironmentQuery::init_generators() {
	UtilityFunctions::print("Initializing generators.");
	for (Variant child : get_children()) {
		QueryGenerator3D *generator = cast_to<QueryGenerator3D>(child);
		if (!generator) {
			print_error("EnvironmentQuery::init_generators(): Child is not a Generator");
			continue;
		}
		generator->set_query_items_ref(query_items);
		generator->connect("generator_finished", callable_mp(this, &EnvironmentQuery::_on_generator_finished));
		generators.push_back(generator);
	}
}

Ref<QueryResult> EnvironmentQuery::get_result() {
	return stored_result;
}

void EnvironmentQuery::set_time_budget_ms(const double budget) {
	time_budget_ms = budget;
}

void EnvironmentQuery::set_is_querying(const bool querying) {
	is_querying = querying;
}

void EnvironmentQuery::request_query() {
	// UtilityFunctions::print("EnvironmentQuery::request_query(): Requested a new query.");
	if (is_querying) {
		// print_error("EnvironmentQuery::request_query(): Requested another query while processing.");
		return;
	}
	_start_query();
}

void EnvironmentQuery::_start_query() {
	// Reset all values before processing the query
	_current_generator = 0;
	// UtilityFunctions::print(vformat("Previous vector size: %s", query_items.size()));
	query_items.clear();
	is_querying = true;
	_process_query();
}

void EnvironmentQuery::_process_query() {
	uint64_t _initial_time_usec = Time::get_singleton()->get_ticks_usec();
	generators[_current_generator]->perform_generation(_initial_time_usec, time_budget_ms);
}

void EnvironmentQuery::_on_generator_finished() {
	_current_generator++;
	if (_current_generator >= generators.size()) {
		Ref<QueryResult> result;
		result.instantiate();
		stored_result = result;
		if (debug_spheres) {
			debug_spheres->draw_items(query_items);
		}
		// Give query_items to result for caching
		stored_result->set_items(std::move(query_items));

		UtilityFunctions::print("Finished Query.");
		is_querying = false;
		//emit_signal("query_finished", stored_result);
		// Fast queries might miss the signal before it's caught, so defer it
		call_deferred("emit_signal", "query_finished", stored_result);
		return;
	}

	// Continue to the next generator/tests
	_process_query();
}

void EnvironmentQuery::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery::get_use_debug_shapes);
	//ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery::request_query);
	ClassDB::bind_method(D_METHOD("get_result"), &EnvironmentQuery::get_result);

	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult")));
}
