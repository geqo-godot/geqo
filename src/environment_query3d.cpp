#include "environment_query3d.h"
#include "contexts/query_context3d.h"
#include "generators/query_generator3d.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void EnvironmentQuery3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!get_querier()) {
				Node3D *new_querier = Object::cast_to<Node3D>(get_owner());
				if (new_querier)
					set_querier(new_querier);
			}
		} break;
		case NOTIFICATION_READY: {
			if (Engine::get_singleton()->is_editor_hint()) {
				update_configuration_warnings();
				return;
			}
			if (!is_inside_tree())
				return;
			if (get_use_debug_shapes()) {
				debug_spheres = memnew(GEQODebugSpheres3D);
				call_deferred("add_sibling", debug_spheres);
			}
			connect("query_finished", callable_mp(GEQODebug::get_singleton(), &GEQODebug::_on_query_finished3d));
			init_generator();
			init_tests();
		} break;
		case NOTIFICATION_CHILD_ORDER_CHANGED: {
			if (Engine::get_singleton()->is_editor_hint())
				update_configuration_warnings();
			break;
		}
	}
}

PackedStringArray EnvironmentQuery3D::_get_configuration_warnings() const {
	PackedStringArray warnings;

	if (get_children().is_empty())
		warnings.append("Must have one QueryGenerator3D child.");
	else {
		bool has_generator = false;
		bool has_context = false;
		int generator_amount = 0;
		for (Variant child : get_children()) {
			QueryGenerator3D *casted_generator = cast_to<QueryGenerator3D>(child);
			if (casted_generator) {
				has_generator = true;
				generator_amount++;
				if (generator_amount == 2) {
					warnings.append("EnvironmentQuery should have one QueryGenerator child, subsequent generators are ignored.");
				}
				continue;
			}
			QueryContext3D *casted_context = cast_to<QueryContext3D>(child);
			if (casted_context)
				has_context = true;
		}
		if (!has_generator)
			warnings.append("Missing a QueryGenerator3D.");
		if (!has_context)
			warnings.append("This query has no QueryContext3Ds");
	}
	if (!get_querier()) {
		warnings.append("EnvironmentQuery has no querier. Assign a Node3D as the owner");
	}

	return warnings;
}

void EnvironmentQuery3D::init_generator() {
	//UtilityFunctions::print("Initializing generators.");
	bool has_generator = true;
	for (Variant child : get_children()) {
		QueryGenerator3D *curr_generator = cast_to<QueryGenerator3D>(child);
		if (curr_generator) {
			curr_generator->set_query_instance(get_query_instance());
			curr_generator->connect("generator_finished", callable_mp(this, &EnvironmentQuery3D::on_generator_finished));
			generator = curr_generator;
			break;
		}
	}
}

void EnvironmentQuery3D::init_tests() {
	if (!_get_generator())
		return;
	_gather_tests();
	for (QueryTest3D *test : _get_sorted_tests()) {
		UtilityFunctions::print(test);
		test->connect("test_finished", callable_mp(this, &EnvironmentQuery3D::on_test_finished));
	}
}

void EnvironmentQuery3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_querier"), &EnvironmentQuery3D::set_querier);
	ClassDB::bind_method(D_METHOD("get_querier"), &EnvironmentQuery3D::get_querier);

	ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery3D::get_use_debug_shapes);
	ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery3D::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery3D::request_query);
	ClassDB::bind_method(D_METHOD("get_result"), &EnvironmentQuery3D::get_result);
	ClassDB::bind_method(D_METHOD("set_time_budget_ms"), &EnvironmentQuery3D::set_time_budget_ms);
	ClassDB::bind_method(D_METHOD("get_time_budget_ms"), &EnvironmentQuery3D::get_time_budget_ms);
	ClassDB::bind_method(D_METHOD("on_test_finished"), &EnvironmentQuery3D::on_test_finished);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "querier", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_querier", "get_querier");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_budget_ms"), "set_time_budget_ms", "get_time_budget_ms");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");

	ADD_SIGNAL(MethodInfo("tests_finished"));
	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult3D")));
}
