#pragma once
#include "geqo_debug_spheres.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
class EnvironmentQuery : public Node {
	GDCLASS(EnvironmentQuery, Node)

private:
	bool use_debug_shapes = false;
	std::vector<QueryItem> query_items;
	GEQODebugSpheres *debug_spheres = nullptr;

public:
	EnvironmentQuery() {}
	~EnvironmentQuery() {}

	void set_use_debug_shapes(const bool use_debug);
	bool get_use_debug_shapes() const { return use_debug_shapes; }

	void _ready() override;

	Ref<QueryResult> request_query();

protected:
	static void _bind_methods();
};
