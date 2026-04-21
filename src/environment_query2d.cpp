#include "environment_query2d.h"
#include "contexts/query_context2d.h"
#include "generators/query_generator2d.h"
#include <contexts/context_target_node2d.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void EnvironmentQuery2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (!get_querier()) {
				Node2D *new_querier = Object::cast_to<Node2D>(get_parent());
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
			if (get_use_debug_shapes()) {
				debug_spheres = memnew(GEQODebugSpheres2D);
				call_deferred("add_sibling", debug_spheres);
			}

			// Add a context node containing the querier
			if (!Engine::get_singleton()->is_editor_hint()) {
				set_querier_context(memnew(ContextTargetNode2D));
				add_child(get_querier_context());
				get_querier_context()->set_target_node(querier);
				get_querier_context()->set_name("ContextQuerier");
			}

			connect("query_finished", callable_mp(GEQODebug::get_singleton(), &GEQODebug::_on_query_finished2d));
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

PackedStringArray EnvironmentQuery2D::_get_configuration_warnings() const {
	PackedStringArray warnings;

	if (get_children().is_empty())
		warnings.append("Must have one QueryGenerator2D child.");
	else {
		bool has_generator = false;
		bool has_context = false;
		int generator_amount = 0;
		for (Variant child : get_children()) {
			QueryGenerator2D *casted_generator = cast_to<QueryGenerator2D>(child);
			if (casted_generator) {
				has_generator = true;
				generator_amount++;
				if (generator_amount == 2) {
					warnings.append("EnvironmentQuery should have one QueryGenerator child, subsequent generators are ignored.");
				}
				continue;
			}
			QueryContext2D *casted_context = cast_to<QueryContext2D>(child);
			if (casted_context)
				has_context = true;
		}
		if (!has_generator)
			warnings.append("Missing a QueryGenerator2D.");
		if (!has_context)
			warnings.append("This query has no QueryContext2Ds");
	}
	if (!get_querier()) {
		warnings.append("EnvironmentQuery has no querier. Assign a Node2D as the owner");
	}

	return warnings;
}

void EnvironmentQuery2D::init_generator() {
	bool has_generator = true;
	for (Variant child : get_children()) {
		QueryGenerator2D *curr_generator = cast_to<QueryGenerator2D>(child);
		if (curr_generator) {
			curr_generator->connect("generator_finished", callable_mp(this, &EnvironmentQuery2D::on_generator_finished));
			generator = curr_generator;
			break;
		}
	}
}

void EnvironmentQuery2D::init_tests() {
	if (!_get_generator())
		return;
	_gather_tests();
	for (QueryTest2D *test : _get_sorted_tests()) {
		test->connect("test_finished", callable_mp(this, &EnvironmentQuery2D::on_test_finished));
	}
}

void EnvironmentQuery2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_querier"), &EnvironmentQuery2D::set_querier);
	ClassDB::bind_method(D_METHOD("get_querier"), &EnvironmentQuery2D::get_querier);

	ClassDB::bind_method(D_METHOD("get_use_debug_shapes"), &EnvironmentQuery2D::get_use_debug_shapes);
	ClassDB::bind_method(D_METHOD("set_use_debug_shapes", "use_debug"), &EnvironmentQuery2D::set_use_debug_shapes);

	ClassDB::bind_method(D_METHOD("request_query"), &EnvironmentQuery2D::request_query);
	ClassDB::bind_method(D_METHOD("get_result"), &EnvironmentQuery2D::get_result);
	ClassDB::bind_method(D_METHOD("set_time_budget_ms"), &EnvironmentQuery2D::set_time_budget_ms);
	ClassDB::bind_method(D_METHOD("get_time_budget_ms"), &EnvironmentQuery2D::get_time_budget_ms);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "querier", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_querier", "get_querier");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time_budget_ms"), "set_time_budget_ms", "get_time_budget_ms");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug_shapes"), "set_use_debug_shapes", "get_use_debug_shapes");

	ADD_SIGNAL(MethodInfo("query_finished", PropertyInfo(Variant::OBJECT, "result", PROPERTY_HINT_RESOURCE_TYPE, "QueryResult2D")));
}
