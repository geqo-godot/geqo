#pragma once
#include "contexts/query_context2d.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
using namespace godot;

class ContextTargetNode2D : public QueryContext2D {
	GDCLASS(ContextTargetNode2D, QueryContext2D)

private:
	Node2D *target_node = nullptr;

public:
	ContextTargetNode2D() {}
	~ContextTargetNode2D() {}

	void set_target_node(Node2D *target);
	Node2D *get_target_node() { return target_node; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
