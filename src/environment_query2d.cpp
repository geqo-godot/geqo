#include "environment_query2d.h"
#include "generators/query_generator2d.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

//void EnvironmentQuery3D::set_use_debug_shapes(const bool use_debug) {
//	use_debug_shapes = use_debug;
//}
void EnvironmentQuery2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			if (Engine::get_singleton()->is_editor_hint()) {
				update_configuration_warnings();
				return;
			}
			if (!is_inside_tree())
				return;
			if (get_use_debug_shapes()) {
				debug_spheres = memnew(GEQODebugSpheres2D);
				call_deferred("add_sibling", debug_spheres);
			}
			init_generator();
		} break;
		case NOTIFICATION_CHILD_ORDER_CHANGED: {
			if (Engine::get_singleton()->is_editor_hint())
				update_configuration_warnings();
			break;
		}
	}
}

PackedStringArray EnvironmentQuery2D::_get_configuration_warnings() const {
	PackedStringArray warnings;

	if (get_children().is_empty())
		warnings.append("Must have one QueryGenerator2D child.");
	else if (get_children().size() > 1)
		warnings.append("EnvironmentQuery should have only one child.");
	else {
		QueryGenerator2D *casted_generator = Object::cast_to<QueryGenerator2D>(get_children()[0]);
		if (!casted_generator)
			warnings.append("Child should be a QueryGenerator2D");
	}

	return warnings;
}
void EnvironmentQuery2D::init_generator() {
	//UtilityFunctions::print("Initializing generators.");
	for (Variant child : get_children()) {
		QueryGenerator2D *curr_generator = cast_to<QueryGenerator2D>(child);
		if (!curr_generator) {
			print_error("EnvironmentQuery::init_generator(): Child is not a Generator");
			continue;
		}
		curr_generator->set_query_items_ref(query_items);
		curr_generator->connect("generator_finished", callable_mp(this, &EnvironmentQuery2D::on_generator_finished));
		generator = curr_generator;
		break;
	}
}

void EnvironmentQuery2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery2D::get_use_debug_shapes);
	ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery2D::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery2D::request_query);
	ClassDB::bind_method(D_METHOD("get_result"), &EnvironmentQuery2D::get_result);
	ClassDB::bind_method(D_METHOD("set_time_budget_ms"), &EnvironmentQuery2D::set_time_budget_ms);
	ClassDB::bind_method(D_METHOD("get_time_budget_ms"), &EnvironmentQuery2D::get_time_budget_ms);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_budget_ms"), "set_time_budget_ms", "get_time_budget_ms");

	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult2D")));
}
