#pragma once
#include "contexts/query_context2d.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
using namespace godot;

class ContextTargetArray2D : public QueryContext2D {
	GDCLASS(ContextTargetArray2D, QueryContext2D)

private:
	TypedArray<Node2D> target_nodes;

public:
	ContextTargetArray2D() {}
	~ContextTargetArray2D() {}

	void set_target_nodes(TypedArray<Node2D> target);
	TypedArray<Node2D> get_target_node() { return target_nodes; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
