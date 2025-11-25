#include "query_test3d.h"

void CQueryTest3D::set_test_purpose(const TestPurpose purpose) {
	test_purpose = purpose;
}

void CQueryTest3D::set_context_operator(const ScoreOperator score_op) {
	multiple_context_operator = score_op;
}

void CQueryTest3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_context_operator"), &CQueryTest3D::get_context_operator);
	ClassDB::bind_method(D_METHOD("set_context_operator", "score_op"), &CQueryTest3D::set_context_operator);

	ClassDB::bind_method(D_METHOD("get_test_purpose"), &CQueryTest3D::get_test_purpose);
	ClassDB::bind_method(D_METHOD("set_test_purpose", "purpose"), &CQueryTest3D::set_test_purpose);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_purpose", PROPERTY_HINT_ENUM, "Filter Score,Filter Only,Score Only"), "set_test_purpose", "get_test_purpose");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_operator", PROPERTY_HINT_ENUM, "Average Score,Max Score,Min Score"), "set_context_operator", "get_context_operator");
}