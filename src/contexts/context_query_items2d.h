#pragma once
#include "contexts/query_context2d.h"
using namespace godot;

class ContextQueryItems2D : public QueryContext2D {
	GDCLASS(ContextQueryItems2D, QueryContext2D)
public:
	ContextQueryItems2D() {}
	~ContextQueryItems2D() {}

	Array get_context(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};
