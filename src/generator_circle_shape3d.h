#pragma once
#include "query_context3d.h"
#include "query_generator3d.h"
#include <vector>
using namespace godot;
class CGeneratorCircleShape3D : public CQueryGenerator3D {
	GDCLASS(CGeneratorCircleShape3D, CQueryGenerator3D)

public:
	CGeneratorCircleShape3D() {}
	~CGeneratorCircleShape3D() {}

	void perform_generation(std::vector<CQueryItem> &query_item_list) override;

protected:
	static void _bind_methods();
};