#pragma once
#include "debug/geqo_debug_spheres.h"
#include "generators/query_generator3d.h"
#include "query_result.h"
#include <algorithm>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/time.hpp>
#include <tuple>
#include <vector>
using namespace godot;
template <typename Traits>
class EnvironmentQueryBase {
protected:
	using VectorT = typename Traits::VectorT;
	using ResultT = typename Traits::ResultT;
	using GeneratorT = typename Traits::GeneratorT;
	using SpheresT = typename Traits::SpheresT;
	using QueryItemT = typename Traits::QueryItemT;
	using QueryTestT = typename Traits::QueryTestT;
	//Current query items of the query.
	std::vector<Ref<QueryItemT>> query_items;
	GeneratorT *generator;
	SpheresT *debug_spheres = nullptr;

	double time_budget_ms = 1.0;

	bool is_querying = false;
	bool use_debug_shapes = false;

	Ref<ResultT> stored_result;

public:
	~EnvironmentQueryBase() = default;

	virtual void init_generator() = 0;

	void _set_use_debug_shapes(const bool use_debug) { use_debug_shapes = use_debug; }
	bool _get_use_debug_shapes() const { return use_debug_shapes; }
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
		if (!generator) {
			print_error("EnvironmentQuery: No Generator in EnvironmentQuery.");
			return;
		}
		_start_query();
	} // Can't be binded from here so must be binded on inherited

	void _start_query() {
		// UtilityFunctions::print(vformat("Previous vector size: %s", query_items.size()));
		query_items.clear();
		is_querying = true;
		_process_query();
	}

	void _process_query() {
		uint64_t _initial_time_usec = Time::get_singleton()->get_ticks_usec();
		generator->perform_generation(_initial_time_usec, time_budget_ms);
	}

	void _on_generator_finished() {
		// Start tests
		perform_tests();
	}

	bool _on_tests_finished() {
		// Process the results
		Ref<ResultT> result;
		result.instantiate();
		stored_result = result;
		if (debug_spheres) {
			debug_spheres->draw_items(query_items);
		}
		// Give query_items to result for caching
		stored_result->set_items(std::move(query_items));

		is_querying = false;
		return true;
	}

	virtual void perform_tests() = 0;

	void _perform_tests() {
		std::vector<QueryTestT *> tests;
		for (Variant test : generator->get_children()) {
			// Make sure all of them are tests
			QueryTestT *current_test = Object::cast_to<QueryTestT>(test);
			if (current_test)
				tests.push_back(current_test);
		}

		// Sort test by purpose, then cost
		std::sort(tests.begin(), tests.end(),
				  [](QueryTestT *a, QueryTestT *b) {
					  if (a->get_test_purpose() != b->get_test_purpose())
						  return a->get_test_purpose() < b->get_test_purpose();
					  return a->get_cost() < b->get_cost();
				  });

		for (Variant test : tests) {
			QueryTestT *current_test = Object::cast_to<QueryTestT>(test);
			for (Ref<QueryItemT> item : query_items) {
				if (item->get_is_filtered())
					continue;
				current_test->perform_test(item);
			}
		}
	}
};
