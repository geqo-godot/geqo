#pragma once
#include "contexts/query_context3d.h"
using namespace godot;

class ContextQueryItems3D : public QueryContext3D {
	GDCLASS(ContextQueryItems3D, QueryContext3D)
public:
	ContextQueryItems3D() {}
	~ContextQueryItems3D() {}

	Array get_context() override;

protected:
	static void _bind_methods();
};
