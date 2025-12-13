#pragma once
#include "debug/geqo_debug_spheres.h"
#include "generators/query_generator3d.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/time.hpp>
#include <vector>
using namespace godot;
template <typename VectorT, typename ResultT, typename GeneratorT, typename SpheresT>
class EnvironmentQueryBase {
protected:
	//Current query items of the query.
	std::vector<QueryItem<VectorT>> query_items;
	std::vector<GeneratorT *> generators;
	SpheresT *debug_spheres = nullptr;

	double time_budget_ms = 1.0;

	bool is_querying = false;

	int _current_generator = 0;
	Ref<ResultT> stored_result;

public:
	~EnvironmentQueryBase() = default;

	virtual void init_generators() = 0;

	//void set_use_debug_shapes(const bool use_debug);
	//bool get_use_debug_shapes() const { return use_debug_shapes; }
	Ref<ResultT> _get_result() {
		return stored_result;
	}

	void _set_time_budget_ms(const double budget) {
		time_budget_ms = budget;
	}
	double _get_time_budget_ms() const { return time_budget_ms; }

	void _set_is_querying(const bool querying) {
		is_querying = querying;
	}
	bool _get_is_querying() const { return is_querying; }

	void _request_query() {
		// UtilityFunctions::print("EnvironmentQuery3D::request_query(): Requested a new query.");
		if (is_querying) {
			// print_error("EnvironmentQuery3D::request_query(): Requested another query while processing.");
			return;
		}
		_start_query();
	} // Can't be binded from here so must be binded on inherited

	void _start_query() {
		// Reset all values before processing the query
		_current_generator = 0;
		// UtilityFunctions::print(vformat("Previous vector size: %s", query_items.size()));
		query_items.clear();
		is_querying = true;
		_process_query();
	}

	void _process_query() {
		uint64_t _initial_time_usec = Time::get_singleton()->get_ticks_usec();
		generators[_current_generator]->perform_generation(_initial_time_usec, time_budget_ms);
	}

	bool _on_generator_finished() {
		_current_generator++;
		if (_current_generator >= generators.size()) {
			Ref<ResultT> result;
			result.instantiate();
			stored_result = result;
			if (debug_spheres) {
				debug_spheres->draw_items(query_items);
			}
			// Give query_items to result for caching
			stored_result->set_items(std::move(query_items));

			UtilityFunctions::print("Finished Query.");
			is_querying = false;
			//emit_signal("query_finished", stored_result);
			return true;
		}

		// Continue to the next generator/tests
		_process_query();
		return false;
	}
};
