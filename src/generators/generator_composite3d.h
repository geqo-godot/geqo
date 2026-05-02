#pragma once
#include "generators/query_generator3d.h"
#include "query_instance.h"

#include <vector>

using namespace godot;

class GeneratorComposite3D : public QueryGenerator3D {
	GDCLASS(GeneratorComposite3D, QueryGenerator3D)
private:
	Callable composite_generator_finished_callable;
	int current_generator = 0;
	std::vector<QueryGenerator3D *> saved_generators; // Saves the generators until the generations are done.
	Ref<QueryInstance3D> saved_instance;

public:
	GeneratorComposite3D() {
		composite_generator_finished_callable = callable_mp(this, &GeneratorComposite3D::_on_composite_generator_finished);
	}
	~GeneratorComposite3D() {}

	void perform_generation(Ref<QueryInstance3D> query_instance) override;
	void _on_composite_generator_finished();

protected:
	static void _bind_methods();
};