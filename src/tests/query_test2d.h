#pragma once
#include "tests/query_test.h"
#include "query_item.h"
#include <godot_cpp/classes/node2d.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;
class QueryTest2D : public Node2D, public QueryTestBase<QueryTest2D> {
	GDCLASS(QueryTest2D, Node2D)

public:
	QueryTest2D() {}
	~QueryTest2D() {}

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

	void set_context_score_operator(const MultipleContextScoreOp score_op) {
		_set_context_score_operator(score_op);
	}
	MultipleContextScoreOp get_context_score_operator() const { return _get_context_score_operator(); }

	void set_context_filter_operator(const MultipleContextFilterOp filter_op) {
		_set_context_filter_operator(filter_op);
	}
	MultipleContextFilterOp get_context_filter_operator() const { return _get_context_filter_operator(); }

	void set_bool_match(const bool match) { _set_bool_match(match); }
	bool get_bool_match() const { return _get_bool_match(); }

	void set_filter_type(FilterType type) {
		_set_filter_type(type);
		notify_property_list_changed();
	}
	FilterType get_filter_type() const { return _get_filter_type(); }

	void set_filter_min(float val) { _set_filter_min(val); }
	float get_filter_min() const { return _get_filter_min(); }

	void set_filter_max(float val) { _set_filter_max(val); }
	float get_filter_max() const { return _get_filter_max(); }

	void set_scoring_factor(float val) { _set_scoring_factor(val); }
	float get_scoring_factor() const { return _get_scoring_factor(); }

	void set_cost(float new_cost) { return _set_cost(new_cost); }
	float get_cost() { return _get_cost(); }

	virtual void perform_test(Ref<QueryItem2D> projection);
	GDVIRTUAL1_REQUIRED(_perform_test, Ref<QueryItem2D>)

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;
};

VARIANT_ENUM_CAST(QueryTest2D::TestPurpose);
VARIANT_ENUM_CAST(QueryTest2D::TestType);
VARIANT_ENUM_CAST(QueryTest2D::FilterType);
VARIANT_ENUM_CAST(QueryTest2D::MultipleContextFilterOp);
VARIANT_ENUM_CAST(QueryTest2D::MultipleContextScoreOp);
