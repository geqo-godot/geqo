#include "tests/test_distance_to3d.h"
#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <numeric>
using namespace godot;

void TestDistanceTo3D::set_scoring_curve(Ref<Curve> curve) {
	scoring_curve = curve;
}

void TestDistanceTo3D::set_distance_to(QueryContext3D *context_node) {
	distance_to = context_node;
}

void TestDistanceTo3D::perform_test(Ref<QueryInstance3D> query_instance) {
	//// UtilityFunctions::print_rich("Testing the tested test to test");
	//if (distance_to == nullptr) {
	//	UtilityFunctions::print_rich("Test has no context");
	//	return;
	//}

	//Array context_positions = distance_to->get_context_positions();

	//// Collect raw distances
	//std::vector<double> distances;
	//for (int i = 0; i < context_positions.size(); i++) {
	//	Vector3 pos = context_positions[i];
	//	distances.push_back(projection->get_projection_position().distance_to(pos));
	//}

	//// Aggregate distances based on operator
	//double final_raw_distance = 0.0;
	//double highest_distance = *std::max_element(distances.begin(), distances.end());
	//double lowest_distance = *std::min_element(distances.begin(), distances.end());
	//switch (get_context_score_operator()) {
	//	case OP_AVERAGE_SCORE:
	//		final_raw_distance = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
	//		break;
	//	case OP_MAX_SCORE:
	//		final_raw_distance = highest_distance;
	//		break;
	//	case OP_MIN_SCORE:
	//		final_raw_distance = lowest_distance;
	//		break;
	//}

	//// Handle filtering using raw distance against thresholds
	//if (get_test_purpose() == FILTER_ONLY || get_test_purpose() == FILTER_SCORE) {
	//	projection->add_score_numeric(
	//			FILTER_ONLY,
	//			get_filter_type(),
	//			final_raw_distance,
	//			(get_filter_type() == GEQOEnums::FILTER_TYPE_MIN || get_filter_type() == GEQOEnums::FILTER_TYPE_RANGE) ? get_filter_min() : 0.0,
	//			(get_filter_type() == GEQOEnums::FILTER_TYPE_MAX || get_filter_type() == GEQOEnums::FILTER_TYPE_RANGE) ? get_filter_max() : highest_distance);
	//}

	//// If filtered, no point scoring
	//if (projection->get_is_filtered())
	//	return;

	//// Handle scoring using curve-adjusted value
	//if (get_test_purpose() == SCORE_ONLY || get_test_purpose() == FILTER_SCORE) {
	//	double normalized = 0.0;
	//	switch (get_filter_type()) {
	//		case FILTER_TYPE_MIN: {
	//			double range = highest_distance - get_filter_min();
	//			normalized = (range > 0.0) ? std::clamp((final_raw_distance - get_filter_min()) / range, 0.0, 1.0) : 0.0;
	//			break;
	//		}
	//		case FILTER_TYPE_MAX: {
	//			normalized = (get_filter_max() > 0.0) ? std::clamp(final_raw_distance / get_filter_max(), 0.0, 1.0) : 0.0;
	//			break;
	//		}
	//		case FILTER_TYPE_RANGE: {
	//			double range = get_filter_max() - get_filter_min();
	//			normalized = (range > 0.0) ? std::clamp((final_raw_distance - get_filter_min()) / range, 0.0, 1.0) : 0.0;
	//			break;
	//		}
	//	}

	//	double curve_score = scoring_curve->sample_baked(normalized);
	//	projection->add_score_numeric(
	//			SCORE_ONLY,
	//			get_filter_type(),
	//			curve_score,
	//			0.0,
	//			1.0);
	//}
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