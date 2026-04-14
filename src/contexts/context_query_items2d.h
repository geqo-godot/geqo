#pragma once
#include "contexts/query_context2d.h"
using namespace godot;

class ContextQueryItems2D : public QueryContext2D {
	GDCLASS(ContextQueryItems2D, QueryContext2D)
private:
	bool use_current_query_item = true;

public:
	ContextQueryItems2D() {}
	~ContextQueryItems2D() {}

	bool get_use_current_query_item() { return use_current_query_item; }
	void set_use_current_query_item(bool use);

	Array get_context(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};
