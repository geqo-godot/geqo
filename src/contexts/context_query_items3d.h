#pragma once
#include "contexts/query_context3d.h"
using namespace godot;

class ContextQueryItems3D : public QueryContext3D {
	GDCLASS(ContextQueryItems3D, QueryContext3D)
private:
	bool use_current_query_item = true;

public:
	ContextQueryItems3D() {}
	~ContextQueryItems3D() {}

	bool get_use_current_query_item() { return use_current_query_item; }
	void set_use_current_query_item(bool use);

	Array get_context(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};
