#pragma once
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
class QueryTest3D : public Node3D {
	GDCLASS(QueryTest3D, Node3D)

public:
	enum TestPurpose {
		FILTER_SCORE,
		FILTER_ONLY,
		SCORE_ONLY,
	};
	enum ScoreOperator {
		AVERAGE_SCORE,
		MAX_SCORE,
		MIN_SCORE,
	};

private:
	TestPurpose test_purpose = FILTER_SCORE;
	ScoreOperator multiple_context_operator = AVERAGE_SCORE;

public:
	QueryTest3D() {}
	~QueryTest3D() {}

	void set_test_purpose(const TestPurpose purpose);
	TestPurpose get_test_purpose() const { return test_purpose; }

	void set_context_operator(const ScoreOperator score_op);
	ScoreOperator get_context_operator() const { return multiple_context_operator; }

	virtual void perform_test(QueryItem &projection) = 0;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryTest3D::TestPurpose);
VARIANT_ENUM_CAST(QueryTest3D::ScoreOperator);
