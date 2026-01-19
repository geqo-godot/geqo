#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>

using namespace godot;
class TestPathFindTo3D : public QueryTest3D {
	GDCLASS(TestPathFindTo3D, QueryTest3D)

public:
	enum PathTestType {
		PATH_EXISTS,
		PATH_COST
	};

private:
	Ref<Curve> scoring_curve;
	PathTestType path_test_type = PATH_EXISTS;
	QueryContext3D *path_to = nullptr;
	double found_path_threshold = 0.1;

public:
	TestPathFindTo3D() {}
	~TestPathFindTo3D() {}

	void set_scoring_curve(Ref<Curve> curve);
	Ref<Curve> get_scoring_curve() const { return scoring_curve; }

	void set_path_to(QueryContext3D *context_node);
	QueryContext3D *get_path_to() { return path_to; }

	void set_found_path_threshold(double threshold);
	double get_found_path_threshold() { return found_path_threshold; }

	void perform_test(QueryItem<Vector3> &projection) override;

	PackedVector3Array get_navigation_path(Vector3 p_start_position, Vector3 p_target_position);
	void _ready() override;
	void draw_path(const PackedVector3Array &path, Color color);

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(TestPathFindTo3D::PathTestType);