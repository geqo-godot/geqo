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
		case NOTIFICATION_PROCESS: {
			if (Engine::get_singleton()->is_editor_hint())
				return;
			if (is_querying) {
				if (_process_query()) {
					// Query finished with true so don't query anymore
					is_querying = false;
				}
			}
		} break;
	}
}

void EnvironmentQuery::init_generators() {
	for (Variant child : get_children()) {
		QueryGenerator3D *generator = cast_to<QueryGenerator3D>(child);
		if (!generator) {
			print_error("EnvironmentQuery::init_generators(): Child is not a Generator");
			continue;
		}
		generators.push_back(generator);
	}
}

void EnvironmentQuery::set_time_budget_ms(const double budget) {
	time_budget_ms = budget;
}

void EnvironmentQuery::set_is_querying(const bool querying) {
	is_querying = querying;
}

void EnvironmentQuery::request_query() {
	if (is_querying)
		print_error("EnvironmentQuery::request_query(): Requested another query while processing.");
	return;
	_start_query();
}

void EnvironmentQuery::_start_query() {
	// Reset all values before processing the query
	_current_generator = 0;
	_current_test = 0;
	query_items.clear();

	_initial_time_usec = Time::get_singleton()->get_ticks_usec();
	is_querying = true;
}

bool EnvironmentQuery::_process_query() {
	for (int gen = _current_generator; gen < generators.size(); gen++) {
		if (!generators[gen]) {
			print_error("EnvironmentQuery: Child is not Generator");
			continue;
		}
		generators[gen]->perform_generation(query_items);
		generators[gen]->perform_tests(query_items);
	}

	Ref<QueryResult> result;
	result.instantiate();
	result->set_items(query_items);

	if (debug_spheres) {
		debug_spheres->draw_items(query_items);
	}
	emit_signal("query_finished", result);
	return true;
}

void EnvironmentQuery::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery::get_use_debug_shapes);
	//ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery::request_query);

	//ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result")));
}
