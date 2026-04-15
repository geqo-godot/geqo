#include "tests/test_dot_product2d.h"
#include "contexts/context_target_node2d.h"
#include "test_dot_product2d.h"

void TestDotProduct2D::set_line_a_mode(LineMode mode) {
	line_a_mode = mode;
}

void TestDotProduct2D::set_line_a_rotation(QueryContext2D *context) {
	line_a_rotation = context;
}

void TestDotProduct2D::set_line_a_from(QueryContext2D *context) {
	line_a_from = context;
}

void TestDotProduct2D::set_line_a_to(QueryContext2D *context) {
	line_a_to = context;
}

void TestDotProduct2D::set_line_b_mode(LineMode mode) {
	line_b_mode = mode;
}

void TestDotProduct2D::set_line_b_rotation(QueryContext2D *context) {
	line_b_rotation = context;
}

void TestDotProduct2D::set_line_b_from(QueryContext2D *context) {
	line_b_from = context;
}

void TestDotProduct2D::set_line_b_to(QueryContext2D *context) {
	line_b_to = context;
}

void TestDotProduct2D::set_use_absolute_value(bool use) {
	use_absolute_value = use;
}

Vector2 TestDotProduct2D::get_line(
		LineMode mode,
		QueryContext2D *rotation,
		QueryContext2D *from,
		QueryContext2D *to,
		Ref<QueryInstance2D> query_instance) {
	switch (mode) {
		case LINE_ROTATION: {
			Array nodes = rotation
					? rotation->get_context_nodes(query_instance)
					: query_instance->get_querier_context()->get_context_nodes(query_instance);

			if (nodes.is_empty())
				return Vector2();

			std::vector<Vector2> forwards;
			forwards.reserve(nodes.size());

			Vector2 sum;

			for (Variant node : nodes) {
				Node2D *n = Object::cast_to<Node2D>(node);
				if (!n)
					continue;

				Vector2 f = Vector2::from_angle(n->get_global_rotation());
				forwards.push_back(f);
				sum += f;
			}

			if (sum.length_squared() == 0)
				return Vector2();

			Vector2 average = sum.normalized();

			switch (get_multiple_context_score_operator()) {
				case GEQOEnums::OP_AVERAGE_SCORE:
					return average;

				case GEQOEnums::OP_MIN_SCORE: {
					Vector2 best;
					float best_dot = std::numeric_limits<float>::max();

					for (const Vector2 &f : forwards) {
						float d = average.dot(f);
						if (d < best_dot) {
							best_dot = d;
							best = f;
						}
					}
					return best;
				}

				case GEQOEnums::OP_MAX_SCORE: {
					Vector2 best;
					float best_dot = std::numeric_limits<float>::lowest();

					for (const Vector2 &f : forwards) {
						float d = average.dot(f);
						if (d > best_dot) {
							best_dot = d;
							best = f;
						}
					}
					return best;
				}
			}
		} break;

		case LINE_TWO_POINTS: {
		} break;
	}

	return Vector2();
}

Vector2 TestDotProduct2D::get_line_a(Ref<QueryInstance2D> query_instance) {
	return get_line(line_a_mode, line_a_rotation, line_a_from, line_a_to, query_instance);
}

Vector2 TestDotProduct2D::get_line_b(Ref<QueryInstance2D> query_instance) {
	return get_line(line_b_mode, line_b_rotation, line_b_from, line_b_to, query_instance);
}

void TestDotProduct2D::perform_test(Ref<QueryInstance2D> query_instance) {
	Vector2 line_a = get_line_a(query_instance);
	Vector2 line_b = get_line_b(query_instance);

	// Invalid
	if (line_a.length_squared() == 0 || line_b.length_squared() == 0) {
		end_test();
		return;
	}

	line_a = line_a.normalized();
	line_b = line_b.normalized();

	while (query_instance->has_items()) {
		Ref<QueryItem2D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		double dot_product = line_a.dot(line_b);

		if (use_absolute_value)
			dot_product = UtilityFunctions::absf(dot_product);

		// Filtering
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_FILTER_ONLY) {
			if (!item->apply_filter_numeric(
						get_filter_type(),
						dot_product,
						get_filter_min(),
						get_filter_max()))
				continue;
		}

		// Scoring
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_SCORE ||
			get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY) {
			double clamp_min = get_score_clamp_min();
			double clamp_max = get_score_clamp_max();

			double normalized = std::clamp(
					(dot_product - clamp_min) /
							std::max(clamp_max - clamp_min, 1e-9),
					0.0,
					1.0);

			double curve_score = get_scoring_curve()->sample_baked(normalized);

			item->add_score_direct(
					get_test_purpose(),
					curve_score,
					get_scoring_factor());
		}
	}

	end_test();
}
void TestDotProduct2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_line_a_mode", "mode"), &TestDotProduct2D::set_line_a_mode);
	ClassDB::bind_method(D_METHOD("get_line_a_mode"), &TestDotProduct2D::get_line_a_mode);
	ClassDB::bind_method(D_METHOD("set_line_a_rotation", "context"), &TestDotProduct2D::set_line_a_rotation);
	ClassDB::bind_method(D_METHOD("get_line_a_rotation"), &TestDotProduct2D::get_line_a_rotation);
	ClassDB::bind_method(D_METHOD("set_line_a_from", "context"), &TestDotProduct2D::set_line_a_from);
	ClassDB::bind_method(D_METHOD("get_line_a_from"), &TestDotProduct2D::get_line_a_from);
	ClassDB::bind_method(D_METHOD("set_line_a_to", "context"), &TestDotProduct2D::set_line_a_to);
	ClassDB::bind_method(D_METHOD("get_line_a_to"), &TestDotProduct2D::get_line_a_to);

	ClassDB::bind_method(D_METHOD("set_line_b_mode", "mode"), &TestDotProduct2D::set_line_b_mode);
	ClassDB::bind_method(D_METHOD("get_line_b_mode"), &TestDotProduct2D::get_line_b_mode);
	ClassDB::bind_method(D_METHOD("set_line_b_rotation", "context"), &TestDotProduct2D::set_line_b_rotation);
	ClassDB::bind_method(D_METHOD("get_line_b_rotation"), &TestDotProduct2D::get_line_b_rotation);
	ClassDB::bind_method(D_METHOD("set_line_b_from", "context"), &TestDotProduct2D::set_line_b_from);
	ClassDB::bind_method(D_METHOD("get_line_b_from"), &TestDotProduct2D::get_line_b_from);
	ClassDB::bind_method(D_METHOD("set_line_b_to", "context"), &TestDotProduct2D::set_line_b_to);
	ClassDB::bind_method(D_METHOD("get_line_b_to"), &TestDotProduct2D::get_line_b_to);

	ClassDB::bind_method(D_METHOD("set_use_absolute_value", "use"), &TestDotProduct2D::set_use_absolute_value);
	ClassDB::bind_method(D_METHOD("get_use_absolute_value"), &TestDotProduct2D::get_use_absolute_value);

	BIND_ENUM_CONSTANT(LINE_ROTATION);
	BIND_ENUM_CONSTANT(LINE_TWO_POINTS);

	//ADD_PROPERTY(PropertyInfo(Variant::INT, "line_a_mode", PROPERTY_HINT_ENUM, "Rotation,TwoPoints"), "set_line_a_mode", "get_line_a_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_rotation", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_a_rotation", "get_line_a_rotation");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_from", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_a_from", "get_line_a_from");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_a_to", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_a_to", "get_line_a_to");

	//ADD_PROPERTY(PropertyInfo(Variant::INT, "line_b_mode", PROPERTY_HINT_ENUM, "Rotation,TwoPoints"), "set_line_b_mode", "get_line_b_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_rotation", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_b_rotation", "get_line_b_rotation");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_from", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_b_from", "get_line_b_from");
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "line_b_to", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_line_b_to", "get_line_b_to");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_absolute_value"), "set_use_absolute_value", "get_use_absolute_value");
}