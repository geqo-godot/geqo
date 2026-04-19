#include "debug/geqo_debug_spheres3d.h"
#include <godot_cpp/classes/label3d.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>

#include "geqo_debug_spheres3d.h"
#include "query_result.h"

Array GEQODebugSpheres3D::get_debug_mesh_lines() const {
	float r = 0.5;

	PackedVector3Array points;

	for (int i = 0; i <= 360; i++) {
		float ra = Math::deg_to_rad((float)i);
		float rb = Math::deg_to_rad((float)i + 1);
		Point2 a = Vector2(Math::sin(ra), Math::cos(ra)) * r;
		Point2 b = Vector2(Math::sin(rb), Math::cos(rb)) * r;

		points.push_back(Vector3(a.x, 0, a.y));
		points.push_back(Vector3(b.x, 0, b.y));
		points.push_back(Vector3(0, a.x, a.y));
		points.push_back(Vector3(0, b.x, b.y));
		points.push_back(Vector3(a.x, a.y, 0));
		points.push_back(Vector3(b.x, b.y, 0));
	}

	// Resize it to match ARRAY_MAX because add_surface_from_arrays() demanded it
	Array arr;
	arr.resize(Mesh::ARRAY_MAX);
	arr[Mesh::ARRAY_VERTEX] = points;

	return arr;
}

void GEQODebugSpheres3D::clear_spheres() {
	sphere_data.clear();
	// TODO: Clear them from rendering as well
}

void GEQODebugSpheres3D::render_spheres() {
	if (!multi_mesh_instance) {
		multi_mesh_instance = memnew(MultiMeshInstance3D);
		add_child(multi_mesh_instance);
	}
	if (!line_multi_mesh_instance) {
		line_multi_mesh_instance = memnew(MultiMeshInstance3D);
		add_child(line_multi_mesh_instance);
	}

	if (!sphere_mesh.is_valid()) {
		sphere_mesh.instantiate();

		sphere_mesh->set_radius(0.5);
		sphere_mesh->set_height(1.0);

		sphere_mesh->set_radial_segments(12);
		sphere_mesh->set_rings(6);
		sphere_mesh->set_material(get_sphere_material());
	}
	if (!line_mesh.is_valid()) {
		line_mesh.instantiate();
		line_mesh->add_surface_from_arrays(Mesh::PRIMITIVE_LINES, get_debug_mesh_lines());
		line_mesh->surface_set_material(0, get_line_material());
	}

	// First, the inner transparent mesh
	Ref<MultiMesh> sphere_mm;
	Ref<MultiMesh> line_mm;
	sphere_mm.instantiate();

	sphere_mm->set_mesh(sphere_mesh);
	sphere_mm->set_transform_format(MultiMesh::TRANSFORM_3D);
	sphere_mm->set_use_colors(true);
	sphere_mm->set_use_custom_data(false);
	sphere_mm->set_instance_count(sphere_data.size());

	for (int32_t i = 0; i < sphere_data.size(); i++) {
		Transform3D transform = Transform3D();
		transform.set_origin(sphere_data[i].position);
		sphere_mm->set_instance_transform(i, transform);
		sphere_mm->set_instance_color(i, sphere_data[i].color);
	}

	line_mm = sphere_mm->duplicate_deep();
	line_mm->set_mesh(line_mesh);
	multi_mesh_instance->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
	multi_mesh_instance->set_multimesh(sphere_mm);
	line_multi_mesh_instance->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
	line_multi_mesh_instance->set_multimesh(line_mm);
}

void GEQODebugSpheres3D::draw_items(std::vector<Ref<QueryItem3D>> &query_items_list, double time_to_destroy) {
	remove_labels();
	clear_spheres();

	for (Ref<QueryItem3D> query_item : query_items_list) {
		Label3D *text_label = memnew(Label3D);
		text_label->set_draw_flag(Label3D::FLAG_DISABLE_DEPTH_TEST, true);
		text_labels.append(text_label);
		add_child(text_label);
		text_label->set_deferred("billboard", BaseMaterial3D::BILLBOARD_ENABLED);
		text_label->set_deferred("position", Vector3(0, 1, 0) * 0.75);

		if (query_item->get_is_filtered()) {
			text_label->set_deferred("text", "Filtered");
			draw_debug_sphere(query_item->get_projection_position(), Color(0, 0, 1));
		} else {
			if (query_item->get_has_score()) {
				text_label->set_deferred("text", String::num(query_item->get_score()).pad_decimals(2));
				draw_debug_sphere(query_item->get_projection_position(), debug_color->sample(query_item->get_score()));
			} else {
				draw_debug_sphere(query_item->get_projection_position(), Color(0, 1, 1));
			}
		}
		text_label->set_deferred("global_position", query_item->get_projection_position());
	}
	render_spheres();
}

void GEQODebugSpheres3D::draw_debug_sphere(Vector3 pos, Color color) {
	sphere_data.push_back(SphereData(pos, color));
}

Ref<StandardMaterial3D> GEQODebugSpheres3D::get_sphere_material() {
	if (sphere_material.is_valid())
		return sphere_material;
	Ref<StandardMaterial3D> material;
	material.instantiate();
	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	material->set_depth_draw_mode(BaseMaterial3D::DEPTH_DRAW_DISABLED);
	material->set_albedo(Color(1, 1, 1, 0.35));
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	sphere_material = material;
	return sphere_material;
}

Ref<StandardMaterial3D> GEQODebugSpheres3D::get_line_material() {
	if (line_material.is_valid())
		return line_material;
	Ref<StandardMaterial3D> material;
	material.instantiate();
	material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);

	line_material = material;
	return line_material;
}

void GEQODebugSpheres3D::_ready() {
	debug_color = Ref<Gradient>();
	debug_color.instantiate();
	PackedColorArray colors = PackedColorArray();
	// Red
	colors.append(Color(1.0, 0.0, 0.0));
	// Yellow
	colors.append(Color(1.0, 1.0, 0.0));
	// Green
	colors.append(Color(0.0, 1.0, 0.0));

	debug_color->set_colors(colors);

	PackedFloat32Array offsets = PackedFloat32Array();
	offsets.append(0.0);
	offsets.append(0.5);
	offsets.append(1.0);

	debug_color->set_offsets(offsets);

	debug_color->set_interpolation_mode(Gradient::GRADIENT_INTERPOLATE_CUBIC);
}

void GEQODebugSpheres3D::_bind_methods() {
}
