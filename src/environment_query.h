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
	//bool use_debug_shapes = false;
	std::vector<QueryItem> query_items;
	std::vector<QueryGenerator3D *> generators;
	GEQODebugSpheres *debug_spheres = nullptr;

	double time_budget_ms = 1.0;

	bool is_querying = false;

	int _current_generator = 0;
	int _current_test = 0;
	uint64_t _initial_time_usec = 0;

public:
	EnvironmentQuery() {}
	~EnvironmentQuery() {}

	void init_generators();

	//void set_use_debug_shapes(const bool use_debug);
	//bool get_use_debug_shapes() const { return use_debug_shapes; }

	void set_time_budget_ms(const double budget);
	double get_time_budget_ms() const { return time_budget_ms; }

	void set_is_querying(const bool querying);
	bool get_is_querying() const { return is_querying; }

	void request_query();
	void _start_query();
	bool _process_query();

protected:
	static void _bind_methods();
	void _notification(int p_what);
};
