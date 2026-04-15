#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class TestDistanceTo2D : public QueryTest2D {
	GDCLASS(TestDistanceTo2D, QueryTest2D)

private:
	QueryContext2D *distance_to = nullptr;
	Ref<QueryInstance2D> stored_instance;

public:
	TestDistanceTo2D() : QueryTest2D() {
		set_cost(1.29);
		set_test_type(GEQOEnums::TEST_TYPE_NUMERIC);
		set_score_clamp_max(200.0);
		set_filter_max(50.0);
	}
	~TestDistanceTo2D() {}

	void set_distance_to(QueryContext2D *context_node);
	QueryContext2D *get_distance_to() { return distance_to; }

	double calculate_context_score(Ref<QueryItem2D> item, const Array &context_positions);
	bool evaluate_context_filter_any(Ref<QueryItem2D> item, const Array &context_positions);
	bool evaluate_context_filter_all(Ref<QueryItem2D> item, const Array &context_positions);
	double get_effective_clamp_min(Ref<QueryInstance2D> query_instance);
	double get_effective_clamp_max(Ref<QueryInstance2D> query_instance);

	void _on_next_process_frame();
	void perform_test(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};