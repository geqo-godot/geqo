#pragma once
#include "debug/geqo_debug_spheres.h"
#include "query_result.h"
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <vector>
using namespace godot;

struct SphereData {
	Vector3 position;
	Color color;

	SphereData(Vector3 pos, Color col) {
		position = pos;
		color = col;
	}
};

class GEQODebugSpheres3D : public Node, public GEQODebugSpheresBase<Vector3, QueryItem3D> {
	GDCLASS(GEQODebugSpheres3D, Node)

private:
	std::vector<SphereData> sphere_data;
	MultiMeshInstance3D *multi_mesh_instance = nullptr;
	MultiMeshInstance3D *line_multi_mesh_instance = nullptr;
	Ref<SphereMesh> sphere_mesh;
	Ref<ArrayMesh> line_mesh;
	Ref<StandardMaterial3D> sphere_material;
	Ref<StandardMaterial3D> line_material;

public:
	GEQODebugSpheres3D() {}
	~GEQODebugSpheres3D() {}

	void draw_items(std::vector<Ref<QueryItem3D>> &query_items_list, double time_to_destroy = 2.0);
	void draw_debug_sphere(Vector3 pos, Color color);
	Ref<StandardMaterial3D> get_sphere_material();
	Ref<StandardMaterial3D> get_line_material();
	Array get_debug_mesh_lines() const;
	void clear_spheres();
	void render_spheres();
	void _ready() override;

protected:
	static void _bind_methods();
};