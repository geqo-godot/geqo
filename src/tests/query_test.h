#pragma once
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;

template <typename Derived>
class QueryTestBase {
public:
	using TestPurpose = GEQOEnums::TestPurpose;
	using TestType = GEQOEnums::TestType;
	using FilterType = GEQOEnums::FilterType;
	using MultipleContextScoreOp = GEQOEnums::MultipleContextScoreOp;
	using MultipleContextFilterOp = GEQOEnums::MultipleContextFilterOp;

private:
	TestPurpose test_purpose = GEQOEnums::PURPOSE_FILTER_SCORE;
	TestType test_type = GEQOEnums::TEST_TYPE_NUMERIC;
	MultipleContextScoreOp multiple_context_score_operator = GEQOEnums::OP_AVERAGE_SCORE;
	MultipleContextFilterOp multiple_context_filter_operator = GEQOEnums::OP_ANY_PASS;
	FilterType filter_type = GEQOEnums::FILTER_TYPE_MIN;
	float filter_min = 0.0f;
	float filter_max = 0.0f;
	float scoring_factor = 1.0f;

	// Used when test type is boolean
	bool bool_match = false;
	float cost = 0.0;

public:
	~QueryTestBase() = default;

	void _set_test_purpose(const TestPurpose purpose) {
		test_purpose = purpose;
	}
	TestPurpose _get_test_purpose() const { return test_purpose; }

	void _set_test_type(TestType type) { test_type = type; }
	TestType _get_test_type() const { return test_type; }

	void _set_context_score_operator(const MultipleContextScoreOp score_op) {
		multiple_context_score_operator = score_op;
	}
	MultipleContextScoreOp _get_context_score_operator() const { return multiple_context_score_operator; }

	void _set_context_filter_operator(const MultipleContextFilterOp filter_op) {
		multiple_context_filter_operator = filter_op;
	}
	MultipleContextFilterOp _get_context_filter_operator() const { return multiple_context_filter_operator; }

	void _set_bool_match(const bool match) { bool_match = match; }
	bool _get_bool_match() const { return bool_match; }

	void _set_filter_type(FilterType type) { filter_type = type; }
	FilterType _get_filter_type() const { return filter_type; }

	void _set_filter_min(float val) { filter_min = val; }
	float _get_filter_min() const { return filter_min; }

	void _set_filter_max(float val) { filter_max = val; }
	float _get_filter_max() const { return filter_max; }

	void _set_scoring_factor(float val) { scoring_factor = val; }
	float _get_scoring_factor() const { return scoring_factor; }

	void _set_cost(const float new_cost) {
		cost = new_cost;
	}
	float _get_cost() const {
		return cost;
	}
};
