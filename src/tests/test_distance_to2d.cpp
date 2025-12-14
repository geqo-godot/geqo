
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

void TestDistanceTo2D::perform_test(QueryItem<Vector2> &projection) {
	// UtilityFunctions::print_rich("Testing the tested test to test");
	if (distance_to == nullptr) {
		UtilityFunctions::print_rich("Test has no context");
		return;
	}

	Array context_positions = distance_to->get_context_positions();
	std::vector<double> scores = {};

	for (Variant context_pos : context_positions) {
		if (context_pos.get_type() != Variant::VECTOR2)
			continue;

		double distance = projection.projection_position.distance_to(context_pos);

		if (get_test_purpose() == FILTER_SCORE || get_test_purpose() == FILTER_ONLY) {
			if (distance < min_distance || distance > max_distance) {
				scores.push_back(0.0);
				continue;
			}
		}

		double linear_score = (distance - min_distance) / (max_distance - min_distance);
		double clamped_score = std::clamp(linear_score, 0.0, 1.0);
		double curve_score = scoring_curve->sample_baked(clamped_score);
		scores.push_back(curve_score);
	}

	double result = 0.0;

	// Choose score for the result
	switch (get_context_operator()) {
		case AVERAGE_SCORE: {
			double total_score = std::accumulate(scores.begin(), scores.end(), 0.0);
			result = total_score / scores.size();
			break;
		}
		case MAX_SCORE:
			result = *std::max(scores.begin(), scores.end());
			break;
		case MIN_SCORE:
			result = *std::min_element(scores.begin(), scores.end());
			break;
	}

	switch (get_test_purpose()) {
		case FILTER_SCORE: {
			if (result == 0.0)
				projection.is_filtered = true;
			else
				projection.add_score(result);
			break;
		}
		case FILTER_ONLY:
			if (result == 0.0)
				projection.is_filtered = true;
			break;
		case SCORE_ONLY:
			projection.add_score(result);
			break;
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