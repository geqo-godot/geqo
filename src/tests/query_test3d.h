#pragma once
#include "query_result.h"
#include "tests/query_test.h"
#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;
class QueryTest3D : public Node3D, public QueryTestBase<Vector3> {
	GDCLASS(QueryTest3D, Node3D)

public:
	QueryTest3D() {}
	~QueryTest3D() {}

	void set_test_purpose(const TestPurpose purpose) {
		notify_property_list_changed();
		return _set_test_purpose(purpose);
	};
	TestPurpose get_test_purpose() const { return _get_test_purpose(); }

	void set_context_score_operator(const ScoreOperator score_op) { return _set_context_score_operator(score_op); };
	ScoreOperator get_context_score_operator() const { return _get_context_score_operator(); }

	void set_context_filter_operator(const FilterOperator filter_op) { return _set_context_filter_operator(filter_op); };
	FilterOperator get_context_filter_operator() const { return _get_context_filter_operator(); }
	void set_cost(float new_cost) { return _set_cost(new_cost); }
	float get_cost() { return _get_cost(); }

	virtual void perform_test(Ref<QueryItem3D> projection);
	GDVIRTUAL1_REQUIRED(_perform_test, Ref<QueryItem3D>)

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;
};

VARIANT_ENUM_CAST(QueryTest3D::TestPurpose);
VARIANT_ENUM_CAST(QueryTest3D::ScoreOperator);
VARIANT_ENUM_CAST(QueryTest3D::FilterOperator);
