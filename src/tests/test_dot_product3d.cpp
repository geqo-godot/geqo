#include "tests/test_dot_product3d.h"
#include "contexts/context_querier3d.h"
#include "test_dot_product3d.h"

void TestDotProduct3D::set_line_a_mode(LineMode mode) {
	line_a_mode = mode;
}

void TestDotProduct3D::set_line_a_rotation(QueryContext3D *context) {
	line_a_rotation = context;
}

void TestDotProduct3D::set_line_a_from(QueryContext3D *context) {
	line_a_from = context;
}

void TestDotProduct3D::set_line_a_to(QueryContext3D *context) {
	line_a_to = context;
}

void TestDotProduct3D::set_line_b_mode(LineMode mode) {
	line_b_mode = mode;
}

void TestDotProduct3D::set_line_b_rotation(QueryContext3D *context) {
	line_b_rotation = context;
}

void TestDotProduct3D::set_line_b_from(QueryContext3D *context) {
	line_b_from = context;
}

void TestDotProduct3D::set_line_b_to(QueryContext3D *context) {
	line_b_to = context;
}

void TestDotProduct3D::set_use_absolute_value(bool use) {
	use_absolute_value = use;
}

Vector3 TestDotProduct3D::get_line(LineMode mode, QueryContext3D *rotation, QueryContext3D *from, QueryContext3D *to, Ref<QueryInstance3D> query_instance) {
	switch (mode) {
		case LINE_ROTATION: {
			Array nodes = rotation ? rotation->get_context_nodes(query_instance) : query_instance->get_querier_context()->get_context_nodes(query_instance);
			Vector3 sum;
			for (Variant node : nodes) {
				Node3D *casted_node = Object::cast_to<Node3D>(node);
				sum += -(casted_node->get_global_transform().get_basis().get_column(2));
			}

			Vector3 average = sum.normalized();

			switch (get_multiple_context_score_operator()) {
				case GEQOEnums::OP_AVERAGE_SCORE:
					return average;
				case GEQOEnums::OP_MIN_SCORE: {
					Vector3 min_dir;
					float min_dot = std::numeric_limits<float>::max();
					for (Variant node : nodes) {
						Node3D *casted_node = Object::cast_to<Node3D>(node);
						Vector3 forward = -(casted_node->get_global_transform().get_basis().get_column(2));
						float d = average.dot(forward);
						if (d < min_dot) {
							min_dot = d;
							min_dir = forward;
						}
					}
					return min_dir;
				}
				case GEQOEnums::OP_MAX_SCORE: {
					Vector3 max_dir;
					float max_dot = std::numeric_limits<float>::lowest();
					for (Variant node : nodes) {
						Node3D *casted_node = Object::cast_to<Node3D>(node);
						Vector3 forward = -(casted_node->get_global_transform().get_basis().get_column(2));
						float d = average.dot(forward);
						if (d > max_dot) {
							max_dot = d;
							max_dir = forward;
						}
					}
					return max_dir;
				}
			}
		} break;
		case LINE_TWO_POINTS: {
		} break;
	}
	return Vector3();
}

Vector3 TestDotProduct3D::get_line_a(Ref<QueryInstance3D> query_instance) {
	return get_line(line_a_mode, line_a_rotation, line_a_from, line_a_to, query_instance);
}

Vector3 TestDotProduct3D::get_line_b(Ref<QueryInstance3D> query_instance) {
	return get_line(line_b_mode, line_b_rotation, line_b_from, line_b_to, query_instance);
}

void TestDotProduct3D::perform_test(Ref<QueryInstance3D> query_instance) {
	Vector3 line_a = get_line_a(query_instance);
	Vector3 line_b = get_line_b(query_instance);

	while (query_instance->has_items()) {
		Ref<QueryItem3D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		double dot_product = line_a.dot(line_b);
		if (use_absolute_value)
			dot_product = UtilityFunctions::absf(dot_product);

		//UtilityFunctions::print("Dot product: ", dot_product);
		// Filtering
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_FILTER_ONLY) {
			if (!item->apply_filter_numeric(get_filter_type(), dot_product, get_filter_min(), get_filter_max()))
				continue;
		}

		// Scoring - normalize then sample curve
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY) {
			double clamp_min = get_score_clamp_min();
			double clamp_max = get_score_clamp_max();
			double normalized = std::clamp((dot_product - clamp_min) / std::max(clamp_max - clamp_min, 1e-9), 0.0, 1.0);
			double curve_score = get_scoring_curve()->sample_baked(normalized);
			item->add_score_direct(get_test_purpose(), curve_score, get_scoring_factor());
		}
	}
	end_test();
}

void TestDotProduct3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_line_a_mode", "mode"), &TestDotProduct3D::set_line_a_mode);
	ClassDB::bind_method(D_METHOD("get_line_a_mode"), &TestDotProduct3D::get_line_a_mode);
	ClassDB::bind_method(D_METHOD("set_line_a_rotation", "context"), &TestDotProduct3D::set_line_a_rotation);
	ClassDB::bind_method(D_METHOD("get_line_a_rotation"), &TestDotProduct3D::get_line_a_rotation);
	ClassDB::bind_method(D_METHOD("set_line_a_from", "context"), &TestDotProduct3D::set_line_a_from);
	ClassDB::bind_method(D_METHOD("get_line_a_from"), &TestDotProduct3D::get_line_a_from);
	ClassDB::bind_method(D_METHOD("set_line_a_to", "context"), &TestDotProduct3D::set_line_a_to);
	ClassDB::bind_method(D_METHOD("get_line_a_to"), &TestDotProduct3D::get_line_a_to);

	ClassDB::bind_method(D_METHOD("set_line_b_mode", "mode"), &TestDotProduct3D::set_line_b_mode);
	ClassDB::bind_method(D_METHOD("get_line_b_mode"), &TestDotProduct3D::get_line_b_mode);
	ClassDB::bind_method(D_METHOD("set_line_b_rotation", "context"), &TestDotProduct3D::set_line_b_rotation);
	ClassDB::bind_method(D_METHOD("get_line_b_rotation"), &TestDotProduct3D::get_line_b_rotation);
	ClassDB::bind_method(D_METHOD("set_line_b_from", "context"), &TestDotProduct3D::set_line_b_from);
	ClassDB::bind_method(D_METHOD("get_line_b_from"), &TestDotProduct3D::get_line_b_from);
	ClassDB::bind_method(D_METHOD("set_line_b_to", "context"), &TestDotProduct3D::set_line_b_to);
	ClassDB::bind_method(D_METHOD("get_line_b_to"), &TestDotProduct3D::get_line_b_to);

	ClassDB::bind_method(D_METHOD("set_use_absolute_value", "use"), &TestDotProduct3D::set_use_absolute_value);
	ClassDB::bind_method(D_METHOD("get_use_absolute_value"), &TestDotProduct3D::get_use_absolute_value);

	BIND_ENUM_CONSTANT(LINE_ROTATION);
	BIND_ENUM_CONSTANT(LINE_TWO_POINTS);

	//ADD_PROPERTY(PropertyInfo(Variant::INT, "line_a_mode", PROPERTY_HINT_ENUM, "Rotation,TwoPoints"), "set_line_a_mode", "get_line_a_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_rotation", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_a_rotation", "get_line_a_rotation");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_from", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_a_from", "get_line_a_from");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_a_to", "get_line_a_to");

	//ADD_PROPERTY(PropertyInfo(Variant::INT, "line_b_mode", PROPERTY_HINT_ENUM, "Rotation,TwoPoints"), "set_line_b_mode", "get_line_b_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_rotation", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_b_rotation", "get_line_b_rotation");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_from", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_b_from", "get_line_b_from");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_line_b_to", "get_line_b_to");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_absolute_value"), "set_use_absolute_value", "get_use_absolute_value");
}