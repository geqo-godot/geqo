#pragma once
#include "contexts/query_context.h"
#include "environment_query3d.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;
class QueryContext3D : public Node3D, public QueryContextBase<Node3D, PackedVector3Array, Variant::VECTOR3, EnvironmentQuery3D> {
	GDCLASS(QueryContext3D, Node3D)

public:
	QueryContext3D() { set_base_owner(this); }
	~QueryContext3D() {}

	Array get_query_items() { return _get_query_items(); }
	virtual Array get_context();
	GDVIRTUAL0RC_REQUIRED(Array, _get_context)

	PackedVector3Array get_context_positions() { return _get_context_positions(); }
	PackedStringArray _get_configuration_warnings() const override;

protected:
	static void _bind_methods();
};