#pragma once
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;

template <typename VectorT>
class QueryTestBase {
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
	~QueryTestBase() = default;

	void _set_test_purpose(const TestPurpose purpose) {
		test_purpose = purpose;
	}
	TestPurpose _get_test_purpose() const { return test_purpose; }

	void _set_context_operator(const ScoreOperator score_op) {
			multiple_context_operator = score_op;
	}
	ScoreOperator _get_context_operator() const { return multiple_context_operator; }
};
