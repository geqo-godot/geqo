#pragma once
#include "contexts/context_target_node3d.h"
#include "debug/geqo_debug_spheres3d.h"
#include "environment_query.h"
#include "generators/query_generator3d.h"
#include "query_instance.h"
#include "query_result.h"
#include "tests/query_test3d.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
struct QueryTraits3D {
	using NodeT = Node3D;
	using VectorT = Vector3;
	using ResultT = QueryResult3D;
	using GeneratorT = QueryGenerator3D;
	using SpheresT = GEQODebugSpheres3D;
	using QueryItemT = QueryItem3D;
	using QueryTestT = QueryTest3D;
	using QueryInstanceT = QueryInstance3D;
	using ContextTargetNodeT = ContextTargetNode3D;
};

class EnvironmentQuery3D : public Node3D, public EnvironmentQueryBase<QueryTraits3D> {
	GDCLASS(EnvironmentQuery3D, Node3D)
private:
	Callable generator_finished_callable;
	Callable test_finished_callable;

public:
	EnvironmentQuery3D() {
		generator_finished_callable = callable_mp(this, &EnvironmentQuery3D::on_generator_finished);
		test_finished_callable = callable_mp(this, &EnvironmentQuery3D::on_test_finished);
	}
	~EnvironmentQuery3D() {}

	void init_generator() override;
	void init_tests() override;

	void set_querier(Node3D *node) {
		_set_querier(node);
		notify_property_list_changed();
		update_configuration_warnings();
	}
	Node3D *get_querier() const { return _get_querier(); }

	void set_querier_context(ContextTargetNode3D *node) { _set_querier_context(node); }
	ContextTargetNode3D *get_querier_context() const { return _get_querier_context(); }

	Ref<QueryInstance3D> get_query_instance() { return _get_query_instance(); }
	void set_use_debug_shapes(const bool use_debug) { _set_use_debug_shapes(use_debug); }
	bool get_use_debug_shapes() const { return _get_use_debug_shapes(); }

	// TODO: Change this when I figure out how to bind them to Godot without forwarding them
	void set_time_budget_ms(const double budget) { _set_time_budget_ms(budget); }
	double get_time_budget_ms() const { return _get_time_budget_ms(); }

	void set_is_querying(const bool querying) { _set_is_querying(querying); }
	bool get_is_querying() const { return _get_is_querying(); }

	bool request_query() {
		init_generator();
		init_tests();
		bool result = _request_query();
		return result;
	}
	Ref<QueryResult3D> get_result() { return _get_result(); }

	void on_generator_finished() {
		bool result = _on_generator_finished();
		// Generator had no tests / failed so finish early
		if (result) {
			call_deferred("emit_signal", "query_finished", stored_result);
			disconnect_signals();
		}
	}

	void on_test_finished() {
		bool result = _on_test_finished();
		// Fast queries might miss the signal before it's caught, so defer it
		if (result) {
			call_deferred("emit_signal", "query_finished", stored_result);
			disconnect_signals();
		}
	}
	PackedStringArray _get_configuration_warnings() const override;
	void disconnect_signals();

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
