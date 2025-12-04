#pragma once
#include "query_context3d.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
using namespace godot;

class ContextTargetNode3D : public QueryContext3D {
	GDCLASS(ContextTargetNode3D, QueryContext3D)

private:
	Node3D *target_node = nullptr;

public:
	ContextTargetNode3D() {}
	~ContextTargetNode3D() {}

	void set_target_node(Node3D *target);
	Node3D *get_target_node() { return target_node; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
