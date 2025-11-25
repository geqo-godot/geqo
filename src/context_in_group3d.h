#pragma once
#include "query_context3d.h"
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;

class CContextInGroup3D : public CQueryContext3D {
	GDCLASS(CContextInGroup3D, CQueryContext3D)

private:
	String group = "";

public:
	CContextInGroup3D() {}
	~CContextInGroup3D() {}

	void set_group(String new_group);
	String get_group() { return group; }

	Array get_context() override;

protected:
	static void _bind_methods();
};
