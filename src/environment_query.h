#pragma once
#include "geqo_debug_spheres.h"
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <vector>
using namespace godot;
class CEnvironmentQuery : public Node {
	GDCLASS(CEnvironmentQuery, Node)

private:
	bool use_debug_shapes = false;
	std::vector<CQueryItem> query_items;
	CGEQODebugSpheres *debug_spheres = nullptr;

public:
	CEnvironmentQuery() {}
	~CEnvironmentQuery() {}

	void set_use_debug_shapes(const bool use_debug);
	bool get_use_debug_shapes() const { return use_debug_shapes; }

	void _ready() override;

	Ref<CQueryResult> request_query();

protected:
	static void _bind_methods();
};
