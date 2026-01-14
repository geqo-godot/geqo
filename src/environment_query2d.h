#pragma once
#include "debug/geqo_debug_spheres2d.h"
#include "environment_query.h"
#include "generators/query_generator2d.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
class EnvironmentQuery2D : public Node2D, public EnvironmentQueryBase<Vector2, QueryResult2D, QueryGenerator2D, GEQODebugSpheres2D> {
	GDCLASS(EnvironmentQuery2D, Node2D)

public:
	EnvironmentQuery2D() {}
	~EnvironmentQuery2D() {}

	void init_generators();

	void set_use_debug_shapes(const bool use_debug) { return _set_use_debug_shapes(use_debug); }
	bool get_use_debug_shapes() const { return _get_use_debug_shapes(); }

	// TODO: Change this when I figure out how to bind them to Godot without forwarding them
	void set_time_budget_ms(const double budget) { return _set_time_budget_ms(budget); }
	double get_time_budget_ms() const { return _get_time_budget_ms(); }

	void set_is_querying(const bool querying) { return _set_is_querying(querying); }
	bool get_is_querying() const { return _get_is_querying(); }

	void request_query() { return _request_query(); }
	Ref<QueryResult2D> get_result() { return _get_result(); }
	void on_generator_finished() {
		bool result = _on_generator_finished();
		// Fast queries might miss the signal before it's caught, so defer it
		if (result)
			call_deferred("emit_signal", "query_finished", stored_result);
	}

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
