#include "environment_query3d.h"
#include "contexts/query_context3d.h"
#include "generators/query_generator3d.h"
#include <contexts/context_querier3d.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void EnvironmentQuery3D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!get_querier()) {
				Node3D *new_querier = Object::cast_to<Node3D>(get_parent());
				if (new_querier)
					set_querier(new_querier);
				notify_property_list_changed();
			}
		} break;
		case NOTIFICATION_READY: {
			if (Engine::get_singleton()->is_editor_hint()) {
				update_configuration_warnings();
				return;
			}
			if (!is_inside_tree())
				return;

			// Add a context node containing the querier (if it doesn't exist)
			if (!Engine::get_singleton()->is_editor_hint()) {
				bool has_querier_context = false;
				for (Variant child : get_children()) {
					ContextQuerier3D *context_querier = cast_to<ContextQuerier3D>(child);
					if (context_querier) {
						has_querier_context = true;
						set_querier_context(context_querier);
						break;
					}
				}
				if (!has_querier_context) {
					set_querier_context(memnew(ContextQuerier3D));
					add_child(get_querier_context());
					get_querier_context()->set_name("ContextQuerier");
				}
			}

			connect("query_finished", callable_mp(GEQODebug::get_singleton(), &GEQODebug::_on_query_finished3d));
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
	bool has_generator = true;
	for (Variant child : get_children()) {
		QueryGenerator3D *curr_generator = cast_to<QueryGenerator3D>(child);
		if (curr_generator) {
			curr_generator->connect("generator_finished", generator_finished_callable);
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
		test->connect("test_finished", test_finished_callable);
	}
}

void EnvironmentQuery3D::disconnect_signals() {
	generator->disconnect("generator_finished", generator_finished_callable);
	for (QueryTest3D *test : _get_sorted_tests()) {
		test->disconnect("test_finished", test_finished_callable);
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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "querier", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_querier", "get_querier");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_budget_ms"), "set_time_budget_ms", "get_time_budget_ms");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");

	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult3D")));
}
