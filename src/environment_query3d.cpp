#include "environment_query3d.h"
#include "generators/query_generator3d.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

//void EnvironmentQuery3D::set_use_debug_shapes(const bool use_debug) {
//	use_debug_shapes = use_debug;
//}
void EnvironmentQuery3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			if (Engine::get_singleton()->is_editor_hint())
				return;
			if (!is_inside_tree())
				return;
			if (get_use_debug_shapes()) {
				debug_spheres = memnew(GEQODebugSpheres3D);
				call_deferred("add_sibling", debug_spheres);
			}
			init_generators();
		} break;
	}
}

void EnvironmentQuery3D::init_generators() {
	//UtilityFunctions::print("Initializing generators.");
	for (Variant child : get_children()) {
		QueryGenerator3D *generator = cast_to<QueryGenerator3D>(child);
		if (!generator) {
			print_error("EnvironmentQuery::init_generators(): Child is not a Generator");
			continue;
		}
		generator->set_query_items_ref(query_items);
		generator->connect("generator_finished", callable_mp(this, &EnvironmentQuery3D::on_generator_finished));
		generators.push_back(generator);
	}
}

void EnvironmentQuery3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery3D::get_use_debug_shapes);
	ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery3D::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery3D::request_query);
	ClassDB::bind_method(D_METHOD("get_result"), &EnvironmentQuery3D::get_result);
	ClassDB::bind_method(D_METHOD("set_time_budget_ms"), &EnvironmentQuery3D::set_time_budget_ms);
	ClassDB::bind_method(D_METHOD("get_time_budget_ms"), &EnvironmentQuery3D::get_time_budget_ms);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_budget_ms"), "set_time_budget_ms", "get_time_budget_ms");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");

	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult")));
}
