#include "environment_query.h"
#include "query_generator3d.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CEnvironmentQuery::set_use_debug_shapes(const bool use_debug) {
	use_debug_shapes = use_debug;
}

void CEnvironmentQuery::_ready() {
	if (Engine::get_singleton()->is_editor_hint())
		return;
	debug_spheres = memnew(CGEQODebugSpheres);
	call_deferred("add_sibling", debug_spheres);
}

Ref<CQueryResult> CEnvironmentQuery::request_query() {
	query_items.clear();

	for (Variant child : get_children()) {
		CQueryGenerator3D *generator = cast_to<CQueryGenerator3D>(child);
		if (generator == nullptr) {
			print_error("EnvironmentQuery: Child is not Generator");
			continue;
		}
		generator->perform_generation(query_items);
		generator->perform_tests(query_items);
	}

	Ref<CQueryResult> result;
	result.instantiate();
	result->set_items(query_items);

	if (use_debug_shapes && debug_spheres) {
		debug_spheres->draw_items(query_items);
	}
	emit_signal("query_finished", result);
	return result;
}

void CEnvironmentQuery::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &CEnvironmentQuery::get_use_debug_shapes);
	ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &CEnvironmentQuery::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &CEnvironmentQuery::request_query);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result")));
}