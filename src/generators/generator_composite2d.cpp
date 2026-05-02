#include <godot_cpp/classes/scene_tree.hpp>

#include "generator_composite2d.h"
#include "generators/generator_composite2d.h"

void GeneratorComposite2D::perform_generation(Ref<QueryInstance2D> query_instance) {
	current_generator = 0;
	saved_generators.clear();
	saved_instance = query_instance;
	// Collect all the current generators and connect to them.
	for (Variant child : get_children()) {
		QueryGenerator2D *generator = cast_to<QueryGenerator2D>(child);
		if (!generator)
			continue;
		saved_generators.push_back(generator);
		generator->connect("generator_finished", composite_generator_finished_callable);
	}
	// Start the first saved generator
	saved_generators[current_generator]->perform_generation(saved_instance);
}

void GeneratorComposite2D::_on_composite_generator_finished() {
	current_generator++;
	if (current_generator >= saved_generators.size()) {
		for (QueryGenerator2D *generator : saved_generators)
			generator->disconnect("generator_finished", composite_generator_finished_callable);
		emit_signal("generator_finished");
		return;
	}
	saved_generators[current_generator]->perform_generation(saved_instance);
}

void GeneratorComposite2D::_bind_methods() {
}
