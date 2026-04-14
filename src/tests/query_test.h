#pragma once
#include <godot_cpp/classes/curve.hpp>
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
	using ScoreClampType = GEQOEnums::ScoreClampType;

private:
	TestPurpose test_purpose = GEQOEnums::PURPOSE_FILTER_SCORE;
	TestType test_type = GEQOEnums::TEST_TYPE_NUMERIC;

	// Scoring
	MultipleContextScoreOp multiple_context_score_operator = GEQOEnums::OP_AVERAGE_SCORE;
	Ref<Curve> scoring_curve;
	ScoreClampType clamp_min_type = GEQOEnums::CLAMP_TYPE_NONE;
	double score_clamp_min = 0.0;
	ScoreClampType clamp_max_type = GEQOEnums::CLAMP_TYPE_NONE;
	double score_clamp_max = 0.0;

	// Filtering
	MultipleContextFilterOp multiple_context_filter_operator = GEQOEnums::OP_ANY_PASS;
	FilterType filter_type = GEQOEnums::FILTER_TYPE_MIN;
	double filter_min = 0.0;
	double filter_max = 0.0;

	double scoring_factor = 1.0;

	// Used when test type is boolean
	bool bool_match = true;
	double cost = 0.0;

public:
	~QueryTestBase() = default;
	void _set_test_purpose(const TestPurpose purpose) { test_purpose = purpose; }
	TestPurpose _get_test_purpose() const { return test_purpose; }
	void _set_test_type(TestType type) { test_type = type; }
	TestType _get_test_type() const { return test_type; }

	// Scoring
	void _set_multiple_context_score_operator(const MultipleContextScoreOp score_op) { multiple_context_score_operator = score_op; }
	MultipleContextScoreOp _get_multiple_context_score_operator() const { return multiple_context_score_operator; }
	void _set_scoring_curve(Ref<Curve> curve) { scoring_curve = curve; }
	Ref<Curve> _get_scoring_curve() const { return scoring_curve; }

	void _set_clamp_min_type(ScoreClampType clamp_type) { clamp_min_type = clamp_type; }
	ScoreClampType _get_clamp_min_type() const { return clamp_min_type; }
	void _set_score_clamp_min(double score) { score_clamp_min = score; }
	double _get_score_clamp_min() const { return score_clamp_min; }

	void _set_clamp_max_type(ScoreClampType clamp_type) { clamp_max_type = clamp_type; }
	ScoreClampType _get_clamp_max_type() const { return clamp_max_type; }
	void _set_score_clamp_max(double score) { score_clamp_max = score; }
	double _get_score_clamp_max() const { return score_clamp_max; }

	// Filtering
	void _set_filter_type(FilterType type) { filter_type = type; }
	FilterType _get_filter_type() const { return filter_type; }

	void _set_multiple_context_filter_operator(const MultipleContextFilterOp filter_op) { multiple_context_filter_operator = filter_op; }
	MultipleContextFilterOp _get_multiple_context_filter_operator() const { return multiple_context_filter_operator; }

	void _set_bool_match(const bool match) { bool_match = match; }
	bool _get_bool_match() const { return bool_match; }

	void _set_filter_min(double val) { filter_min = val; }
	double _get_filter_min() const { return filter_min; }

	void _set_filter_max(double val) { filter_max = val; }
	double _get_filter_max() const { return filter_max; }

	void _set_scoring_factor(double val) { scoring_factor = val; }
	double _get_scoring_factor() const { return scoring_factor; }

	void _set_cost(const double new_cost) { cost = new_cost; }
	double _get_cost() const { return cost; }
};
