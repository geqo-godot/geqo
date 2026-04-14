#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class TestDistanceTo3D : public QueryTest3D {
	GDCLASS(TestDistanceTo3D, QueryTest3D)

private:
	QueryContext3D *distance_to = nullptr;
	Ref<QueryInstance3D> stored_instance;

public:
	TestDistanceTo3D() : QueryTest3D() {
		set_cost(1.39);
		set_test_type(GEQOEnums::TEST_TYPE_NUMERIC);
		set_score_clamp_max(10.0);
		set_filter_max(10.0);
	}
	~TestDistanceTo3D() {}

	void set_distance_to(QueryContext3D *context_node);
	QueryContext3D *get_distance_to() { return distance_to; }

	double calculate_context_score(Ref<QueryItem3D> item, const Array &context_positions);
	bool evaluate_context_filter_any(Ref<QueryItem3D> item, const Array &context_positions);
	bool evaluate_context_filter_all(Ref<QueryItem3D> item, const Array &context_positions);
	double get_effective_clamp_min(Ref<QueryInstance3D> query_instance);
	double get_effective_clamp_max(Ref<QueryInstance3D> query_instance);

	void _on_next_process_frame();
	void perform_test(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};