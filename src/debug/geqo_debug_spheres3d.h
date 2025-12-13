
#pragma once
#include "debug/geqo_debug_spheres.h"
#include "query_result.h"
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <vector>
using std::vector;
using namespace godot;

class GEQODebugSpheres3D : public Node, public GEQODebugSpheresBase<Vector3> {
	GDCLASS(GEQODebugSpheres3D, Node)

private:
	Ref<ImmediateMesh> immediate_mesh;
	MeshInstance3D *mesh_instance = nullptr;
	Vector3 _sphere_point(double radius, double phi, double theta);

public:
	GEQODebugSpheres3D() {}
	~GEQODebugSpheres3D() {}

	void draw_items(vector<QueryItem<Vector3>> &query_items_list, double time_to_destroy = 2.0);
	void draw_debug_sphere(Vector3 pos, double radius, Color color, int rings = 4, int segments = 8);

	void _ready() override;

protected:
	static void _bind_methods();
};