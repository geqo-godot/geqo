#include "tests/query_test2d.h"
#include "query_test2d.h"
#include <godot_cpp/core/class_db.hpp>

void QueryTest2D::perform_test(Ref<QueryItem2D> projection) {
	GDVIRTUAL_CALL(_perform_test, projection);
}

void QueryTest2D::_bind_methods() {
	GDVIRTUAL_BIND(_perform_test, "projection");
	ClassDB::bind_method(D_METHOD("get_context_score_operator"), &QueryTest2D::get_context_score_operator);
	ClassDB::bind_method(D_METHOD("set_context_score_operator", "score_op"), &QueryTest2D::set_context_score_operator);

	ClassDB::bind_method(D_METHOD("get_context_filter_operator"), &QueryTest2D::get_context_filter_operator);
	ClassDB::bind_method(D_METHOD("set_context_filter_operator", "score_op"), &QueryTest2D::set_context_filter_operator);

	ClassDB::bind_method(D_METHOD("get_test_purpose"), &QueryTest2D::get_test_purpose);
	ClassDB::bind_method(D_METHOD("set_test_purpose", "purpose"), &QueryTest2D::set_test_purpose);

	BIND_ENUM_CONSTANT(FILTER_SCORE);
	BIND_ENUM_CONSTANT(FILTER_ONLY);
	BIND_ENUM_CONSTANT(SCORE_ONLY);
	BIND_ENUM_CONSTANT(AVERAGE_SCORE);
	BIND_ENUM_CONSTANT(MAX_SCORE);
	BIND_ENUM_CONSTANT(MIN_SCORE);
	BIND_ENUM_CONSTANT(ANY_PASS);
	BIND_ENUM_CONSTANT(ALL_PASS);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_purpose", PROPERTY_HINT_ENUM, "Filter Score,Filter Only,Score Only"), "set_test_purpose", "get_test_purpose");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_score_operator", PROPERTY_HINT_ENUM, "Average Score,Max Score,Min Score"), "set_context_score_operator", "get_context_score_operator");
}