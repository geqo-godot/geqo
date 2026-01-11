#pragma once
#include "contexts/query_context.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;
class QueryContext2D : public Node2D, QueryContextBase<Node2D, PackedVector2Array, Variant::VECTOR2> {
	GDCLASS(QueryContext2D, Node2D)

public:
	QueryContext2D() { set_base_owner(this); }
	~QueryContext2D() {}

	virtual Array get_context();
	GDVIRTUAL0RC(Array, _get_context)

	PackedVector2Array get_context_positions() { return _get_context_positions(); }

protected:
	static void _bind_methods();
};