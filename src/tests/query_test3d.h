#pragma once
#include "query_result.h"
#include "tests/query_test.h"
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
class QueryTest3D : public Node3D, public QueryTestBase<Vector3> {
	GDCLASS(QueryTest3D, Node3D)

public:
	QueryTest3D() {}
	~QueryTest3D() {}

	void set_test_purpose(const TestPurpose purpose) { return _set_test_purpose(purpose); };
	TestPurpose get_test_purpose() const { return _get_test_purpose(); }

	void set_context_score_operator(const ScoreOperator score_op) { return _set_context_score_operator(score_op); };
	ScoreOperator get_context_score_operator() const { return _get_context_score_operator(); }

	void set_context_filter_operator(const FilterOperator filter_op) { return _set_context_filter_operator(filter_op); };
	FilterOperator get_context_filter_operator() const { return _get_context_filter_operator(); }

	virtual void perform_test(QueryItem<Vector3> &projection) = 0;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryTest3D::TestPurpose);
VARIANT_ENUM_CAST(QueryTest3D::ScoreOperator);
VARIANT_ENUM_CAST(QueryTest3D::FilterOperator);
