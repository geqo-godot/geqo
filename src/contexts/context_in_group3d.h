#pragma once
#include "contexts/query_context3d.h"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;

class ContextInGroup3D : public QueryContext3D {
	GDCLASS(ContextInGroup3D, QueryContext3D)

private:
	String group = "";

public:
	ContextInGroup3D() {}
	~ContextInGroup3D() {}

	void set_group(String new_group);
	String get_group() { return group; }

	Array get_context(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};
