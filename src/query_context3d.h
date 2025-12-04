#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
using namespace godot;
class QueryContext3D : public Node3D {
	GDCLASS(QueryContext3D, Node3D)

public:
	QueryContext3D() {}
	~QueryContext3D() {}

	virtual Array get_context() = 0;
	PackedVector3Array get_context_positions();

protected:
	static void _bind_methods();
};