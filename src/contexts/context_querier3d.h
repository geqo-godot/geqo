#pragma once
#include "contexts/query_context3d.h"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;

class ContextQuerier3D : public QueryContext3D {
	GDCLASS(ContextQuerier3D, QueryContext3D)
public:
	ContextQuerier3D() {}
	~ContextQuerier3D() {}
	Array get_context(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};
