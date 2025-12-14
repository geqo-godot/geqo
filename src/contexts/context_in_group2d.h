#pragma once
#include "contexts/query_context2d.h"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;

class ContextInGroup2D : public QueryContext2D {
	GDCLASS(ContextInGroup2D, QueryContext2D)

private:
	String group = "";

public:
	ContextInGroup2D() {}
	~ContextInGroup2D() {}

	void set_group(String new_group);
	String get_group() { return group; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
