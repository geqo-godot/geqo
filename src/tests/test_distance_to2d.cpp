
#include "tests/test_distance_to2d.h"
#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <numeric>
using namespace godot;

void TestDistanceTo2D::set_scoring_curve(Ref<Curve> curve) {
	scoring_curve = curve;
}

void TestDistanceTo2D::set_distance_to(QueryContext2D *context_node) {
	distance_to = context_node;
}

void TestDistanceTo2D::set_min_distance(double dist) {
	min_distance = dist;
}

void TestDistanceTo2D::set_max_distance(double dist) {
	max_distance = dist;
}

void TestDistanceTo2D::perform_test(Ref<QueryItem2D> projection) {
	// UtilityFunctions::print_rich("Testing the tested test to test");
	if (distance_to == nullptr) {
		UtilityFunctions::print_rich("Test has no context");
		return;
	}

	Array context_positions = distance_to->get_context_positions();

	// Collect raw distances
	std::vector<double> distances;
	for (int i = 0; i < context_positions.size(); i++) {
		Vector2 pos = context_positions[i];
		distances.push_back(projection->get_projection_position().distance_to(pos));
	}

	// Aggregate distances based on operator
	double final_raw_distance = 0.0;
	switch (get_context_score_operator()) {
		case AVERAGE_SCORE:
			final_raw_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
			break;
		case MAX_SCORE:
			final_raw_distance = *std::max_element(distances.begin(), distances.end());
			break;
		case MIN_SCORE:
			final_raw_distance = *std::min_element(distances.begin(), distances.end());
			break;
	}

	// Normalize the distances and sample the curve score
	double range = max_distance - min_distance;
	double normalized = (range > 0.0) ? std::clamp((final_raw_distance - min_distance) / range, 0.0, 1.0) : 0.0;
	double curve_score = scoring_curve->sample_baked(normalized);

	// Add the score
	projection->add_score(
			get_test_purpose(),
			get_context_filter_operator(),
			curve_score,
			0.0,
			1.0);

	// Raw distance was out of bounds so manually filter
	if (final_raw_distance < min_distance || final_raw_distance > max_distance) {
		if (get_test_purpose() != SCORE_ONLY) {
			projection->set_is_filtered(true);
		}
	}
}

void TestDistanceTo2D::_ready() {
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

void TestDistanceTo2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_scoring_curve"), &TestDistanceTo2D::get_scoring_curve);
	ClassDB::bind_method(D_METHOD("set_scoring_curve", "curve"), &TestDistanceTo2D::set_scoring_curve);

	ClassDB::bind_method(D_METHOD("get_distance_to"), &TestDistanceTo2D::get_distance_to);
	ClassDB::bind_method(D_METHOD("set_distance_to", "context_node"), &TestDistanceTo2D::set_distance_to);

	ClassDB::bind_method(D_METHOD("get_min_distance"), &TestDistanceTo2D::get_min_distance);
	ClassDB::bind_method(D_METHOD("set_min_distance", "dist"), &TestDistanceTo2D::set_min_distance);

	ClassDB::bind_method(D_METHOD("get_max_distance"), &TestDistanceTo2D::get_max_distance);
	ClassDB::bind_method(D_METHOD("set_max_distance", "dist"), &TestDistanceTo2D::set_max_distance);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "distance_to", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_distance_to", "get_distance_to");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance"), "set_min_distance", "get_min_distance");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance"), "set_max_distance", "get_max_distance");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scoring_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scoring_curve", "get_scoring_curve");
}