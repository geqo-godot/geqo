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
	using NodeT = typename Traits::NodeT;
	using VectorT = typename Traits::VectorT;
	using ResultT = typename Traits::ResultT;
	using GeneratorT = typename Traits::GeneratorT;
	using SpheresT = typename Traits::SpheresT;
	using QueryItemT = typename Traits::QueryItemT;
	using QueryTestT = typename Traits::QueryTestT;
	using QueryInstanceT = typename Traits::QueryInstanceT;
	using ContextQuerierT = typename Traits::ContextQuerierT;
	using EnvironmentQueryT = typename Traits::EnvironmentQueryT;

	// The parent, aka the node that is inheriting this class
	Object *owner = nullptr;
	Ref<QueryInstanceT> instance;
	NodeT *querier = nullptr;
	ContextQuerierT *querier_context = nullptr;
	GeneratorT *generator = nullptr;
	std::vector<QueryTestT *> sorted_tests;
	int current_test = 0;
	SpheresT *debug_spheres = nullptr;

	double time_budget_ms = 1.0;
	uint64_t last_start_time_usec = 0;

	bool is_querying = false;
	bool use_debug_shapes = false;

	Ref<ResultT> stored_result;

public:
	~EnvironmentQueryBase() = default;

	void set_base_owner(Object *p_owner) {
		owner = p_owner;
	}

	virtual void init_generator() = 0;
	virtual void init_tests() = 0;

	void _set_querier(NodeT *node) { querier = node; }
	NodeT *_get_querier() const { return querier; }

	void _set_querier_context(ContextQuerierT *node) { querier_context = node; }
	ContextQuerierT *_get_querier_context() const { return querier_context; }

	GeneratorT *_get_generator() const { return generator; }

	uint64_t get_last_start_time() { return last_start_time_usec; }
	void set_last_start_time(uint64_t usecs) { last_start_time_usec = usecs; }

	Ref<QueryInstanceT> _get_query_instance() { return instance; }

	void _set_use_debug_shapes(const bool use_debug) { use_debug_shapes = use_debug; }
	bool _get_use_debug_shapes() const { return use_debug_shapes; }
	Ref<ResultT> _get_result() { return stored_result; }

	void _set_time_budget_ms(const double budget) { time_budget_ms = budget; }
	double _get_time_budget_ms() const { return time_budget_ms; }

	void _set_is_querying(const bool querying) { is_querying = querying; }
	bool _get_is_querying() const { return is_querying; }

	bool _request_query() {
		// Returns true if query has started, false if it failed to start

		// UtilityFunctions::print("EnvironmentQuery3D::request_query(): Requested a new query.");
		if (is_querying) {
			// print_error("EnvironmentQuery3D::request_query(): Requested another query while processing.");
			return false;
		}
		if (!generator) {
			print_error("EnvironmentQuery: No Generator in EnvironmentQuery.");
			return false;
		}
		last_start_time_usec = Time::get_singleton()->get_ticks_usec();
		_start_query();
		return true;
	} // Can't be binded from here so must be binded on inherited

	void _start_query() {
		// UtilityFunctions::print(vformat("Previous vector size: %s", query_items.size()));
		instance = Ref<QueryInstanceT>();
		instance.instantiate();
		instance->set_environment_query(Object::cast_to<EnvironmentQueryT>(owner));
		instance->set_querier_context(querier_context);
		instance->set_budget(time_budget_ms);
		is_querying = true;
		current_test = 0;
		_process_query();
	}

	void _process_query() {
		generator->perform_generation(instance);
	}

	bool _on_generator_finished() {
		// Start tests
		return _perform_tests();
	}

	std::vector<QueryTestT *> _get_sorted_tests() {
		return sorted_tests;
	}

	void _gather_tests() {
		sorted_tests.clear();
		for (Variant test : generator->get_children()) {
			// Make sure all of them are tests
			QueryTestT *current_test = Object::cast_to<QueryTestT>(test);
			if (current_test)
				sorted_tests.push_back(current_test);
		}
		// Sort test by purpose, then cost
		std::sort(sorted_tests.begin(), sorted_tests.end(),
				  [](QueryTestT *a, QueryTestT *b) {
					  if (a->get_test_purpose() != b->get_test_purpose())
						  return a->get_test_purpose() < b->get_test_purpose();
					  return a->get_cost() < b->get_cost();
				  });
	}

	bool _perform_tests() {
		if (sorted_tests.size() != generator->get_children().size())
			_gather_tests();

		if (instance->get_item_count() == 0) {
			_finalize_query();
			return true;
		}
		if (sorted_tests.empty()) {
			_finalize_query();
			return true;
		}

		// Begin first test
		QueryTestT *first_test = sorted_tests[0];
		instance->clear_test_data(first_test);
		first_test->perform_test(instance);
		return false;
	}

	// Will return true to do the call_deferred query_finshed on the 2D and 3D environment queries
	bool _on_test_finished() {
		current_test++;
		instance->reset_iterator();
		if (current_test < sorted_tests.size()) {
			QueryTestT *test_instance = sorted_tests[current_test];
			instance->clear_test_data(test_instance);
			test_instance->perform_test(instance);
			return false;
		} else {
			// Normalize scores across all non-filtered items
			std::vector<Ref<QueryItemT>> &items = instance->get_items();
			float highest_score = -std::numeric_limits<float>::infinity();
			float lowest_score = std::numeric_limits<float>::infinity();

			for (Ref<QueryItemT> item : items) {
				if (item->get_is_filtered())
					continue;
				float s = item->get_score();
				if (s > highest_score)
					highest_score = s;
				if (s < lowest_score)
					lowest_score = s;
			}

			if (highest_score == lowest_score) {
				// All unfiltered items scored identically — put them in the middle
				for (Ref<QueryItemT> item : items) {
					if (!item->get_is_filtered())
						item->set_score(0.5);
				}
			} else {
				for (Ref<QueryItemT> item : items) {
					if (item->get_is_filtered())
						continue;
					float normalized = (item->get_score() - lowest_score) / (highest_score - lowest_score);
					item->set_score(normalized);
				}
			}

			_finalize_query();
			return true;
		}
	}
	void _finalize_query() {
		stored_result.instantiate();
		stored_result->set_time_it_took(
				Time::get_singleton()->get_ticks_usec() - last_start_time_usec);

		if (use_debug_shapes) {
			if (!debug_spheres) {
				debug_spheres = memnew(SpheresT);
				Node *parent_env_query = Object::cast_to<Node>(owner);
				parent_env_query->add_child(debug_spheres);
			}
			debug_spheres->draw_items(instance->get_items());
		}

		stored_result->set_items(instance->take_items());
		is_querying = false;
	}
};
