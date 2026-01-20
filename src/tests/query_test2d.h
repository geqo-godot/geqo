#pragma once
#include "query_result.h"
#include "tests/query_test.h"
#include <godot_cpp/classes/node2d.hpp>
using namespace godot;
class QueryTest2D : public Node2D, public QueryTestBase<Vector2> {
	GDCLASS(QueryTest2D, Node2D)

public:
	QueryTest2D() {}
	~QueryTest2D() {}

	void set_test_purpose(const TestPurpose purpose) { return _set_test_purpose(purpose); };
	TestPurpose get_test_purpose() const { return _get_test_purpose(); }

	void set_context_score_operator(const ScoreOperator score_op) { return _set_context_score_operator(score_op); };
	ScoreOperator get_context_score_operator() const { return _get_context_score_operator(); }

	void set_context_filter_operator(const FilterOperator filter_op) { return _set_context_filter_operator(filter_op); };
	FilterOperator get_context_filter_operator() const { return _get_context_filter_operator(); }

	virtual void perform_test(QueryItem<Vector2> &projection) = 0;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryTest2D::TestPurpose);
VARIANT_ENUM_CAST(QueryTest2D::ScoreOperator);
VARIANT_ENUM_CAST(QueryTest2D::FilterOperator);