#pragma once
#include "contexts/query_context3d.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
using namespace godot;

class ContextTargetArray3D : public QueryContext3D {
	GDCLASS(ContextTargetArray3D, QueryContext3D)

private:
	TypedArray<Node3D> target_nodes;

public:
	ContextTargetArray3D() {}
	~ContextTargetArray3D() {}

	void set_target_nodes(TypedArray<Node3D> target);
	TypedArray<Node3D> get_target_node() { return target_nodes; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
