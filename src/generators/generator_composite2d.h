#pragma once
#include "generators/query_generator2d.h"
#include "query_instance.h"

#include <vector>

using namespace godot;

class GeneratorComposite2D : public QueryGenerator2D {
	GDCLASS(GeneratorComposite2D, QueryGenerator2D)
private:
	Callable composite_generator_finished_callable;
	int current_generator = 0;
	std::vector<QueryGenerator2D *> saved_generators; // Saves the generators until the generations are done.
	Ref<QueryInstance2D> saved_instance;

public:
	GeneratorComposite2D() {
		composite_generator_finished_callable = callable_mp(this, &GeneratorComposite2D::_on_composite_generator_finished);
	}
	~GeneratorComposite2D() {}

	void perform_generation(Ref<QueryInstance2D> query_instance) override;
	void _on_composite_generator_finished();

protected:
	static void _bind_methods();
};