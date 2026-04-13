#include "tests/test_distance_to3d.h"
#include "query_enums.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

#include <algorithm>
#include <numeric>
using namespace godot;

void TestDistanceTo3D::set_scoring_curve(Ref<Curve> curve) {
	scoring_curve = curve;
}

void TestDistanceTo3D::set_distance_to(QueryContext3D *context_node) {
	distance_to = context_node;
}

void TestDistanceTo3D::perform_test(Ref<QueryInstance3D> query_instance) {
	if (!distance_to) {
		UtilityFunctions::push_error("TestDistanceTo3D: Test has no context.");
		end_test();
		return;
	}

	Array context_positions = distance_to->get_context_positions(query_instance);
	if (context_positions.is_empty()) {
		end_test();
		return;
	}

	while (query_instance->has_items()) {
		Ref<QueryItem3D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		// Collect distances from item to all context positions
		std::vector<double> distances;
		for (int i = 0; i < context_positions.size(); i++) {
			Vector3 pos = context_positions[i];
			distances.push_back(item->get_projection_position().distance_to(pos));
		}

		// Aggregate distances based on operator
		double distance = 0.0;
		switch (get_multiple_context_score_operator()) {
			case GEQOEnums::OP_AVERAGE_SCORE:
				distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
				break;
			case GEQOEnums::OP_MAX_SCORE:
				distance = *std::max_element(distances.begin(), distances.end());
				break;
			case GEQOEnums::OP_MIN_SCORE:
				distance = *std::min_element(distances.begin(), distances.end());
				break;
		}

		// Apply curve to distance before scoring
		double highest = *std::max_element(distances.begin(), distances.end());
		double range = get_filter_max() - get_filter_min();
		double normalized = (range > 0.0) ? std::clamp((distance - get_filter_min()) / range, 0.0, 1.0) : 0.0;
		double curve_value = scoring_curve->sample_baked(normalized);

		item->add_score_numeric(get_test_purpose(), get_filter_type(), curve_value, get_filter_min(), get_filter_max());

		if (!query_instance->has_time_left()) {
			UtilityFunctions::print("ONTO NEXT FRAME");
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

// TODO: Change to NOTIFICATION_READY
void TestDistanceTo3D::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (scoring_curve.is_null()) {
		scoring_curve = Ref<Curve>();
		scoring_curve.instantiate();
		scoring_curve->add_point(Vector2(0, 0));
		scoring_curve->add_point(Vector2(1, 1));
	}
	scoring_curve->bake();
}

void TestDistanceTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scoring_curve"), &TestDistanceTo3D::get_scoring_curve);
	ClassDB::bind_method(D_METHOD("set_scoring_curve", "curve"), &TestDistanceTo3D::set_scoring_curve);

	ClassDB::bind_method(D_METHOD("get_distance_to"), &TestDistanceTo3D::get_distance_to);
	ClassDB::bind_method(D_METHOD("set_distance_to", "context_node"), &TestDistanceTo3D::set_distance_to);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "distance_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_distance_to", "get_distance_to");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scoring_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scoring_curve", "get_scoring_curve");
}