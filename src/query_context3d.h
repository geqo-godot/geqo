#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
using namespace godot;
class CQueryContext3D : public Node3D {
	GDCLASS(CQueryContext3D, Node3D)

public:
	CQueryContext3D() {}
	~CQueryContext3D() {}

	virtual Array get_context() = 0;
	PackedVector3Array get_context_positions();

protected:
	static void _bind_methods();
};