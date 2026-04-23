#pragma once
#include "contexts/context_target_node2d.h"
#include "debug/geqo_debug_spheres2d.h"
#include "environment_query.h"
#include "generators/query_generator2d.h"
#include "query_instance.h"
#include "query_result.h"
#include "tests/query_test2d.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
struct QueryTraits2D {
	using NodeT = Node2D;
	using VectorT = Vector2;
	using ResultT = QueryResult2D;
	using GeneratorT = QueryGenerator2D;
	using SpheresT = GEQODebugSpheres2D;
	using QueryItemT = QueryItem2D;
	using QueryTestT = QueryTest2D;
	using QueryInstanceT = QueryInstance2D;
	using ContextTargetNodeT = ContextTargetNode2D;
};

class EnvironmentQuery2D : public Node2D, public EnvironmentQueryBase<QueryTraits2D> {
	GDCLASS(EnvironmentQuery2D, Node2D)

public:
	EnvironmentQuery2D() {}
	~EnvironmentQuery2D() {}

	void init_generator() override;
	void init_tests() override;

	void set_querier(Node2D *node) {
		_set_querier(node);
		notify_property_list_changed();
		update_configuration_warnings();
	}
	Node2D *get_querier() const { return _get_querier(); }

	void set_querier_context(ContextTargetNode2D *node) { _set_querier_context(node); }
	ContextTargetNode2D *get_querier_context() const { return _get_querier_context(); }

	Ref<QueryInstance2D> get_query_instance() { return _get_query_instance(); }
	void set_use_debug_shapes(const bool use_debug) { _set_use_debug_shapes(use_debug); }
	bool get_use_debug_shapes() const { return _get_use_debug_shapes(); }

	// TODO: Change this when I figure out how to bind them to Godot without forwarding them
	void set_time_budget_ms(const double budget) { _set_time_budget_ms(budget); }
	double get_time_budget_ms() const { return _get_time_budget_ms(); }

	void set_is_querying(const bool querying) { _set_is_querying(querying); }
	bool get_is_querying() const { return _get_is_querying(); }

	bool request_query() { return _request_query(); }
	Ref<QueryResult2D> get_result() { return _get_result(); }

	void on_generator_finished() {
		bool result = _on_generator_finished();
		// Generator had no tests / failed so finish early
		if (result)
			call_deferred("emit_signal", "query_finished", stored_result);
	}

	void on_test_finished() {
		bool result = _on_test_finished();
		// Fast queries might miss the signal before it's caught, so defer it
		if (result)
			call_deferred("emit_signal", "query_finished", stored_result);
	}
	PackedStringArray _get_configuration_warnings() const override;

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
