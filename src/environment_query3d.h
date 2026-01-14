#pragma once
#include "debug/geqo_debug_spheres3d.h"
#include "environment_query.h"
#include "generators/query_generator3d.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
class EnvironmentQuery3D : public Node3D, public EnvironmentQueryBase<Vector3, QueryResult3D, QueryGenerator3D, GEQODebugSpheres3D> {
	GDCLASS(EnvironmentQuery3D, Node3D)

public:
	EnvironmentQuery3D() {}
	~EnvironmentQuery3D() {}

	void init_generators();

	//void set_use_debug_shapes(const bool use_debug);
	//bool get_use_debug_shapes() const { return use_debug_shapes; }

	// TODO: Change this when I figure out how to bind them to Godot without forwarding them
	void set_time_budget_ms(const double budget) { return _set_time_budget_ms(budget); }
	double get_time_budget_ms() const { return _get_time_budget_ms(); }

	void set_is_querying(const bool querying) { return _set_is_querying(querying); }
	bool get_is_querying() const { return _get_is_querying(); }

	void request_query() { return _request_query(); }
	Ref<QueryResult3D> get_result() { return _get_result(); }
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
