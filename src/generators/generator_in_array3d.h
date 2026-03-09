#pragma once
#include "contexts/query_context3d.h"
#include "generators/query_generator3d.h"
#include <vector>
using namespace godot;
struct GeneratorInArrayState3D {
	int prev_context = 0;
	double time_budget_ms = 0;

	void reset() {
		prev_context = 0;
		time_budget_ms = 0;
	}
};
class GeneratorInArray3D : public QueryGenerator3D {
	GDCLASS(GeneratorInArray3D, QueryGenerator3D)

private:
	// QueryGenerator
	QueryContext3D *context_array = nullptr;

	GeneratorInArrayState3D _current_state = GeneratorInArrayState3D();

public:
	GeneratorInArray3D() {}
	~GeneratorInArray3D() {}

	void set_context_array(QueryContext3D *context);
	QueryContext3D *get_context_array() { return context_array; }

	void perform_generation(uint64_t initial_time_usec, double time_budget_ms) override;
	void _on_next_process_frame();

protected:
	static void _bind_methods();
};