#pragma once
#include "contexts/query_context2d.h"
#include "generators/query_generator2d.h"
#include "query_instance.h"
#include <vector>
using namespace godot;
struct GeneratorInArrayState2D {
	int prev_context = 0;

	void reset() {
		prev_context = 0;
	}
};
class GeneratorInArray2D : public QueryGenerator2D {
	GDCLASS(GeneratorInArray2D, QueryGenerator2D)

private:
	// QueryGenerator
	Ref<QueryInstance2D> saved_instance;
	QueryContext2D *context_array = nullptr;

	GeneratorInArrayState2D _current_state = GeneratorInArrayState2D();

public:
	GeneratorInArray2D() {}
	~GeneratorInArray2D() {}

	void set_context_array(QueryContext2D *context);
	QueryContext2D *get_context_array() { return context_array; }

	void perform_generation(Ref<QueryInstance2D> query_instance) override;
	void _on_next_process_frame();

protected:
	static void _bind_methods();
};