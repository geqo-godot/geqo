#include <debug/geqo_debug_spheres.h>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/color_names.inc.hpp>
#include <godot_cpp/classes/label3d.hpp>
#include <iomanip>

#include "query_result.h"

void CGEQODebugSpheres::_bind_methods()
{
}

Vector3 CGEQODebugSpheres::_sphere_point(double radius, double phi, double theta)
{
    return Vector3(
        radius * sin(phi) * cos(theta),
        radius * cos(phi),
        radius * sin(phi) * sin(theta));
}

CGEQODebugSpheres::CGEQODebugSpheres()
{
}

CGEQODebugSpheres::~CGEQODebugSpheres()
{
}

void CGEQODebugSpheres::draw_items(vector<CQueryItem> &query_items_list, double time_to_destroy)
{
    // TODO: MAX_MESH_SURFACES prevents drawing more circles
    immediate_mesh->clear_surfaces();
    remove_labels();

    for (CQueryItem query_item : query_items_list)
    {
        Label3D *text_label = memnew(Label3D);
        text_labels.append(text_label);
        add_child(text_label);
        text_label->set_deferred("billboard", BaseMaterial3D::BILLBOARD_ENABLED);
        text_label->set_deferred("position", Vector3(0, 1, 0) * 0.75);

        if (query_item.is_filtered)
        {
            text_label->set_deferred("text", "Filtered");
            draw_debug_sphere(query_item.projection_position, 0.5, Color(0, 0, 1));
        }
        else
        {
            if (query_item.has_score)
            {
                // TODO: Set the precision to 2 decimals
                text_label->set_deferred("text", query_item.score);
                draw_debug_sphere(query_item.projection_position, 0.5, debug_color->sample(query_item.score));
            }
            else
            {
                draw_debug_sphere(query_item.projection_position, 0.5, Color(0, 1, 1));
            }
        }
        text_label->set_deferred("global_position", query_item.projection_position + Vector3(0, 0.6, 0));
    }
}

void CGEQODebugSpheres::remove_labels()
{
    for (Variant label : text_labels)
    {
        Label3D *label_ref = Object::cast_to<Label3D>(label);
        if (label_ref == nullptr)
        {
            print_error("Not a label");
            continue;
        }
        label_ref->queue_free();
    }
    text_labels.clear();
}

void CGEQODebugSpheres::draw_debug_sphere(Vector3 pos, double radius, Color color, int rings, int segments)
{
    immediate_mesh->surface_begin(Mesh::PRIMITIVE_LINES);

    for (int ring = 0; ring < rings; ring++)
    {
        double phi = Math_PI * (double)ring / (double)rings;

        for (int seg = 0; seg < segments; seg++)
        {
            double theta = Math_TAU * (double)seg / (double)segments;
            double theta_next = Math_TAU * (double)(seg + 1) / (double)segments;

            // Current ring
            Vector3 p1 = _sphere_point(radius, phi, theta) + pos;
            Vector3 p2 = _sphere_point(radius, phi, theta_next) + pos;

            immediate_mesh->surface_set_color(color);
            immediate_mesh->surface_add_vertex(p1);
            immediate_mesh->surface_add_vertex(p2);
        }
    }

    // Vertical segments (longitude lines)
    for (int seg = 0; seg < segments; seg++)
    {
        double theta = Math_TAU * (double)seg / (double)segments;

        for (int ring = 0; ring < rings; ring++)
        {
            double phi = Math_PI * (double)ring / (double)rings;
            double phi_next = Math_PI * (double)(ring + 1) / (double)rings;

            Vector3 p1 = _sphere_point(radius, phi, theta) + pos;
            Vector3 p2 = _sphere_point(radius, phi_next, theta) + pos;

            immediate_mesh->surface_set_color(color);
            immediate_mesh->surface_add_vertex(p1);
            immediate_mesh->surface_add_vertex(p2);
        }
    }
    immediate_mesh->surface_end();
}

void CGEQODebugSpheres::_ready()
{
    mesh_instance = memnew(MeshInstance3D);
    immediate_mesh = Ref<ImmediateMesh>();
    immediate_mesh.instantiate();
    mesh_instance->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);
    mesh_instance->set_mesh(immediate_mesh);

    // Create unlit material for debug lines
    Ref<StandardMaterial3D> material = Ref<StandardMaterial3D>();
    material.instantiate();
    material->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
    //  This makes 'surface_set_color' work
    material->set_flag(BaseMaterial3D::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
    mesh_instance->set_material_override(material);

    add_child(mesh_instance);

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
