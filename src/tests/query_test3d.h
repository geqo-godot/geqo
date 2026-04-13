#pragma once
#include "query_enums.h"
#include "query_instance.h"
#include "query_item.h"
#include "tests/query_test.h"
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;
class QueryTest3D : public Node3D, public QueryTestBase<QueryTest3D> {
	GDCLASS(QueryTest3D, Node3D)

public:
	QueryTest3D() {}
	~QueryTest3D() {}
	void set_test_purpose(const TestPurpose purpose) {
		_set_test_purpose(purpose);
		notify_property_list_changed();
	}
	TestPurpose get_test_purpose() const { return _get_test_purpose(); }
	void set_test_type(TestType type) {
		_set_test_type(type);
		notify_property_list_changed();
	}
	TestType get_test_type() const { return _get_test_type(); }

	// Scoring
	void set_multiple_context_score_operator(const MultipleContextScoreOp score_op) { _set_multiple_context_score_operator(score_op); }
	MultipleContextScoreOp get_multiple_context_score_operator() const { return _get_multiple_context_score_operator(); }
	void set_scoring_curve(Ref<Curve> curve) { _set_scoring_curve(curve); }
	Ref<Curve> get_scoring_curve() const { return _get_scoring_curve(); }

	void set_clamp_min_type(ScoreClampType clamp_type) {
		_set_clamp_min_type(clamp_type);
		notify_property_list_changed();
	}
	ScoreClampType get_clamp_min_type() const { return _get_clamp_min_type(); }
	void set_score_clamp_min(double score) { _set_score_clamp_min(score); }
	double get_score_clamp_min() const { return _get_score_clamp_min(); }

	void set_clamp_max_type(ScoreClampType clamp_type) {
		_set_clamp_max_type(clamp_type);
		notify_property_list_changed();
	}
	ScoreClampType get_clamp_max_type() const { return _get_clamp_max_type(); }
	void set_score_clamp_max(double score) { _set_score_clamp_max(score); }
	double get_score_clamp_max() const { return _get_score_clamp_max(); }

	// Filtering
	void set_filter_type(FilterType type) {
		_set_filter_type(type);
		notify_property_list_changed();
	}
	FilterType get_filter_type() const { return _get_filter_type(); }

	void set_multiple_context_filter_operator(const MultipleContextFilterOp filter_op) { _set_multiple_context_filter_operator(filter_op); }
	MultipleContextFilterOp get_multiple_context_filter_operator() const { return _get_multiple_context_filter_operator(); }

	void set_bool_match(const bool match) { _set_bool_match(match); }
	bool get_bool_match() const { return _get_bool_match(); }

	void set_filter_min(double val) { _set_filter_min(val); }
	double get_filter_min() const { return _get_filter_min(); }

	void set_filter_max(double val) { _set_filter_max(val); }
	double get_filter_max() const { return _get_filter_max(); }

	void set_scoring_factor(double val) { _set_scoring_factor(val); }
	double get_scoring_factor() const { return _get_scoring_factor(); }

	void set_cost(const double new_cost) { _set_cost(new_cost); }
	double get_cost() const { return _get_cost(); }

	void end_test();
	virtual void perform_test(Ref<QueryInstance3D> query_instance);
	GDVIRTUAL1_REQUIRED(_perform_test, Ref<QueryInstance3D>)

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;
	void _notification(int p_what);
};