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
	enum FilterOperator {
		ANY_PASS,
		ALL_PASS,
	};

private:
	TestPurpose test_purpose = FILTER_SCORE;
	ScoreOperator multiple_context_score_operator = AVERAGE_SCORE;
	FilterOperator multiple_context_filter_operator = ANY_PASS;

public:
	~QueryTestBase() = default;

	void _set_test_purpose(const TestPurpose purpose) {
		test_purpose = purpose;
	}
	TestPurpose _get_test_purpose() const { return test_purpose; }

	void _set_context_score_operator(const ScoreOperator score_op) {
		multiple_context_score_operator = score_op;
	}
	ScoreOperator _get_context_score_operator() const { return multiple_context_score_operator; }

	void _set_context_filter_operator(const FilterOperator filter_op) {
		multiple_context_filter_operator = filter_op;
	}
	FilterOperator _get_context_filter_operator() const { return multiple_context_filter_operator; }
};
