#pragma once
#include "debug/geqo_debug_spheres.h"
#include "generators/query_generator3d.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
class EnvironmentQuery : public Node {
	GDCLASS(EnvironmentQuery, Node)

private:
	//Current query items of the query.
	std::vector<QueryItem> query_items;
	std::vector<QueryGenerator3D *> generators;
	GEQODebugSpheres *debug_spheres = nullptr;

	double time_budget_ms = 1.0;

	bool is_querying = false;

	int _current_generator = 0;
	Ref<QueryResult> stored_result;

public:
	EnvironmentQuery() {}
	~EnvironmentQuery() {}

	void init_generators();

	//void set_use_debug_shapes(const bool use_debug);
	//bool get_use_debug_shapes() const { return use_debug_shapes; }
	Ref<QueryResult> get_result();

	void set_time_budget_ms(const double budget);
	double get_time_budget_ms() const { return time_budget_ms; }

	void set_is_querying(const bool querying);
	bool get_is_querying() const { return is_querying; }

	void request_query();
	void _start_query();
	void _process_query();
	void _on_generator_finished();

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
