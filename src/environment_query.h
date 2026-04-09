#pragma once
#include "debug/geqo_debug.h"
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
	uint64_t last_start_time_usec = 0;

	bool is_querying = false;
	bool use_debug_shapes = false;

	Ref<ResultT> stored_result;

public:
	~EnvironmentQueryBase() = default;

	virtual void init_generator() = 0;

	uint64_t get_last_start_time() { return last_start_time_usec; }
	void set_last_start_time(uint64_t usecs) { last_start_time_usec = usecs; }

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
		GEQODebug *geqo_debug = GEQODebug::get_singleton();
	}
	bool _get_is_querying() const { return is_querying; }

	TypedArray<Ref<QueryItemT>> _get_query_items() {
		Array items;
		for (Ref<QueryItemT> item : query_items) {
			items.append(item);
		}
		return items;
	}

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
		last_start_time_usec = Time::get_singleton()->get_ticks_usec();
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
		stored_result->set_time_it_took(Time::get_singleton()->get_ticks_usec() - last_start_time_usec);
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

		// TODO: Let user switch normalization mode
		if (query_items.empty())
			return;

		float highest_score = -std::numeric_limits<float>::infinity();
		float lowest_score = std::numeric_limits<float>::infinity();
		for (int i = 0; i < query_items.size(); i++) {
			for (Variant test : tests) {
				QueryTestT *current_test = Object::cast_to<QueryTestT>(test);
				current_test->perform_test(query_items[i]);
				if (query_items[i]->get_is_filtered())
					break;
			}
			if (!query_items[i]->get_is_filtered()) {
				float score = query_items[i]->get_score();
				if (score > highest_score)
					highest_score = score;
				if (score < lowest_score)
					lowest_score = score;
			}
		}

		// Normalize the values
		if (highest_score == lowest_score) {
			// All scores are the same, just put them in the middle
			for (Ref<QueryItemT> item : query_items) {
				if (!item->get_is_filtered())
					item->set_score(0.5);
			}
		} else {
			for (Ref<QueryItemT> item : query_items) {
				if (item->get_is_filtered())
					continue;
				item->set_score(UtilityFunctions::remap(item->get_score(), lowest_score, highest_score, 0.0, 1.0));
			}
		}
	}
};
