#include "tests/test_dot_product2d.h"

void TestDotProduct2D::set_from_place(QueryContext2D *context) {
	from_place = context;
}

void TestDotProduct2D::set_to_target(QueryContext2D *context) {
	to_target = context;
}

void TestDotProduct2D::perform_test(Ref<QueryItem2D> projection) {
	Node2D *context1 = Object::cast_to<Node2D>(from_place->get_context()[0]);
	Node2D *context2 = Object::cast_to<Node2D>(to_target->get_context()[0]);

	if (!context1 || !context2) {
		print_error("TestDotProduct2D missing a context");
		return;
	}

	Vector2 line_1 = Vector2(1, 0).rotated(context1->get_global_rotation());
	Vector2 context2_pos = context2->get_global_position();

	Vector2 line_2 = Vector2(1, 0).rotated(context2->get_global_rotation());
	double dot_product = line_1.dot(line_2);

	double score = 0.0;

	if (dot_product >= get_filter_min() && dot_product <= get_filter_max())
		score = 1.0;

	//	switch (get_test_purpose()) {
	//		case FILTER_SCORE: {
	//			if (score > 0.0)
	//				projection->add_score(score);
	//			else
	//				projection->set_is_filtered(true);
	//			break;
	//		}
	//		case FILTER_ONLY:
	//			if (score <= 0.0)
	//				projection->set_is_filtered(true);
	//			break;
	//		case SCORE_ONLY:
	//			projection->add_score(score);
	//			break;
	//	}
}

void TestDotProduct2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_from_place"), &TestDotProduct2D::get_from_place);
	ClassDB::bind_method(D_METHOD("set_from_place", "context"), &TestDotProduct2D::set_from_place);

	ClassDB::bind_method(D_METHOD("get_to_target"), &TestDotProduct2D::get_to_target);
	ClassDB::bind_method(D_METHOD("set_to_target", "context"), &TestDotProduct2D::set_to_target);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "from_place", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_from_place", "get_from_place");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "to_target", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_to_target", "get_to_target");
}