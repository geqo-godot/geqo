#include "test_dot_product3d.h"

void TestDotProduct3D::set_min_value(double value) {
	min_value = value;
}

void TestDotProduct3D::set_max_value(double value) {
	max_value = value;
}

void TestDotProduct3D::set_from_place(QueryContext3D *context) {
	from_place = context;
}

void TestDotProduct3D::set_to_target(QueryContext3D *context) {
	to_target = context;
}

void TestDotProduct3D::perform_test(QueryItem &projection) {
	Node3D *context1 = Object::cast_to<Node3D>(from_place->get_context()[0]);
	Node3D *context2 = Object::cast_to<Node3D>(to_target->get_context()[0]);

	if (context1 == nullptr || context2 == nullptr) {
		print_error("TestDotProduct3D missing a context");
		return;
	}

	Vector3 line_1 = -(context2->get_global_transform().get_basis().get_column(2));
	Vector3 context2_pos = context2->get_global_position();

	Vector3 line_2 = context1->get_global_position().direction_to(context2->get_global_position());
	double dot_product = line_1.dot(line_2);

	double score = 0.0;

	if (dot_product >= min_value && dot_product <= max_value)
		score = 1.0;

	switch (get_test_purpose()) {
		case FILTER_SCORE: {
			if (score > 0.0)
				projection.add_score(score);
			else
				projection.is_filtered = true;
			break;
		}
		case FILTER_ONLY:
			if (score <= 0.0)
				projection.is_filtered = true;
			break;
		case SCORE_ONLY:
			projection.add_score(score);
			break;
	}
}

void TestDotProduct3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_min_value"), &TestDotProduct3D::get_min_value);
	ClassDB::bind_method(D_METHOD("set_min_value", "value"), &TestDotProduct3D::set_min_value);

	ClassDB::bind_method(D_METHOD("get_max_value"), &TestDotProduct3D::get_max_value);
	ClassDB::bind_method(D_METHOD("set_max_value", "value"), &TestDotProduct3D::set_max_value);

	ClassDB::bind_method(D_METHOD("get_from_place"), &TestDotProduct3D::get_from_place);
	ClassDB::bind_method(D_METHOD("set_from_place", "context"), &TestDotProduct3D::set_from_place);

	ClassDB::bind_method(D_METHOD("get_to_target"), &TestDotProduct3D::get_to_target);
	ClassDB::bind_method(D_METHOD("set_to_target", "context"), &TestDotProduct3D::set_to_target);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_value", PROPERTY_HINT_RANGE, "-1.0,1.0,0.05"), "set_min_value", "get_min_value");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_value", PROPERTY_HINT_RANGE, "-1.0,1.0,0.05"), "set_max_value", "get_max_value");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "from_place", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_from_place", "get_from_place");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "to_target", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_to_target", "get_to_target");
}