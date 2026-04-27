#pragma once
#include "contexts/query_context2d.h"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;

class ContextQuerier2D : public QueryContext2D {
	GDCLASS(ContextQuerier2D, QueryContext2D)
public:
	ContextQuerier2D() {}
	~ContextQuerier2D() {}
	Array get_context(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};
