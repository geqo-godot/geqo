#include "tests/test_distance_to3d.h"
#include "contexts/context_querier3d.h"
#include "query_enums.h"
#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <numeric>
using namespace godot;

void TestDistanceTo3D::set_distance_to(QueryContext3D *context_node) {
	distance_to = context_node;
}

double TestDistanceTo3D::calculate_context_score(Ref<QueryItem3D> item, const Array &context_positions) {
	double sum = 0.0;
	double smallest = std::numeric_limits<double>::infinity();
	double biggest = -std::numeric_limits<double>::infinity();

	for (int i = 0; i < context_positions.size(); i++) {
		Vector3 pos = context_positions[i];
		double dist = item->get_projection_position().distance_to(pos);
		sum += dist;
		if (dist < smallest)
			smallest = dist;
		if (dist > biggest)
			biggest = dist;
	}

	switch (get_multiple_context_score_operator()) {
		case GEQOEnums::OP_AVERAGE_SCORE:
			return sum / context_positions.size();
		case GEQOEnums::OP_MIN_SCORE:
			return smallest;
		case GEQOEnums::OP_MAX_SCORE:
			return biggest;
	}
	return 0.0;
}

bool TestDistanceTo3D::evaluate_context_filter_any(Ref<QueryItem3D> item, const Array &context_positions) {
	for (int i = 0; i < context_positions.size(); i++) {
		Vector3 pos = context_positions[i];
		double dist = item->get_projection_position().distance_to(pos);
		if (dist >= get_filter_min() && dist <= get_filter_max())
			return true;
	}
	return false;
}

bool TestDistanceTo3D::evaluate_context_filter_all(Ref<QueryItem3D> item, const Array &context_positions) {
	for (int i = 0; i < context_positions.size(); i++) {
		Vector3 pos = context_positions[i];
		double dist = item->get_projection_position().distance_to(pos);
		if (dist < get_filter_min() || dist > get_filter_max())
			return false;
	}
	return true;
}

double TestDistanceTo3D::get_effective_clamp_min(Ref<QueryInstance3D> query_instance) {
	switch (get_clamp_min_type()) {
		case GEQOEnums::CLAMP_TYPE_VAL:
			return get_score_clamp_min();
		case GEQOEnums::CLAMP_TYPE_FILTER:
			return get_filter_min();
		default:
			return query_instance->get_test_data_min(this);
	}
}

double TestDistanceTo3D::get_effective_clamp_max(Ref<QueryInstance3D> query_instance) {
	switch (get_clamp_max_type()) {
		case GEQOEnums::CLAMP_TYPE_VAL:
			return get_score_clamp_max();
		case GEQOEnums::CLAMP_TYPE_FILTER:
			return get_filter_max();
		default:
			return query_instance->get_test_data_max(this);
	}
}

void TestDistanceTo3D::perform_test(Ref<QueryInstance3D> query_instance) {
	if (!distance_to)
		distance_to = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());

	Array context_positions = distance_to->get_context_positions(query_instance);
	if (context_positions.is_empty()) {
		end_test();
		return;
	}

	// Pre-pass: find min/max raw scores across all items when clamp type is NONE
	bool needs_pre_pass = get_clamp_min_type() == GEQOEnums::CLAMP_TYPE_NONE || get_clamp_max_type() == GEQOEnums::CLAMP_TYPE_NONE;

	if (needs_pre_pass && !query_instance->has_test_data(this)) {
		double smallest = std::numeric_limits<double>::infinity();
		double biggest = -std::numeric_limits<double>::infinity();

		while (query_instance->has_items()) {
			Ref<QueryItem3D> item = query_instance->get_next_item();
			if (item->get_is_filtered())
				continue;

			double raw = calculate_context_score(item, context_positions);
			if (raw < smallest)
				smallest = raw;
			if (raw > biggest)
				biggest = raw;
		}

		query_instance->set_test_data_min(this, smallest);
		query_instance->set_test_data_max(this, biggest);
		query_instance->reset_iterator();
	}

	// Second pass: filter then score
	double clamp_min = get_effective_clamp_min(query_instance);
	double clamp_max = get_effective_clamp_max(query_instance);

	while (query_instance->has_items()) {
		Ref<QueryItem3D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		double raw_score = calculate_context_score(item, context_positions);

		// Filtering uses raw values — do it before curve sampling
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_FILTER_ONLY) {
			switch (get_multiple_context_filter_operator()) {
				case GEQOEnums::OP_ANY_PASS:
					if (!evaluate_context_filter_any(item, context_positions)) {
						item->set_is_filtered(true);
						continue;
					}
					break;
				case GEQOEnums::OP_ALL_PASS:
					if (!evaluate_context_filter_all(item, context_positions)) {
						item->set_is_filtered(true);
						continue;
					}
					break;
				default:
					if (!item->apply_filter_numeric(get_filter_type(), raw_score, get_filter_min(), get_filter_max()))
						continue;
					break;
			}
		}

		// Scoring uses curve-sampled value x score_factor
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY) {
			double normalized = std::clamp((raw_score - clamp_min) / std::max(clamp_max - clamp_min, 1e-9), 0.0, 1.0);
			double curve_score = get_scoring_curve()->sample_baked(normalized);
			item->add_score_direct(get_test_purpose(), curve_score, get_scoring_factor());
		}

		if (!query_instance->has_time_left()) {
			stored_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &TestDistanceTo3D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	end_test();
}

void TestDistanceTo3D::_on_next_process_frame() {
	stored_instance->refresh_timer();
	perform_test(stored_instance);
}

void TestDistanceTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_distance_to"), &TestDistanceTo3D::get_distance_to);
	ClassDB::bind_method(D_METHOD("set_distance_to", "context_node"), &TestDistanceTo3D::set_distance_to);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "distance_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_distance_to", "get_distance_to");
}