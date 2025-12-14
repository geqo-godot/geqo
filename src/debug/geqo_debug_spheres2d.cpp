#include "debug/geqo_debug_spheres2d.h"
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/theme.hpp>
#include <godot_cpp/classes/theme_db.hpp>

#include "geqo_debug_spheres2d.h"
#include "query_result.h"

void GEQODebugSpheres2D::draw_items(vector<QueryItem<Vector2>> &query_items_list, double time_to_destroy) {
	remove_labels();

	debug_draw->set_query_items(query_items_list);
	debug_draw->set_destroy_time(time_to_destroy);

	for (QueryItem<Vector2> &query_item : query_items_list) {
		Label *text_label = memnew(Label);
		text_labels.append(text_label);
		add_child(text_label);

		if (query_item.is_filtered) {
			text_label->set_deferred("text", "Filtered");
		} else if (query_item.has_score) {
			text_label->set_deferred("text", String::num(query_item.score, 2));
		}

		text_label->set_deferred("global_position", query_item.projection_position);
	}

	if (debug_draw) {
		debug_draw->set_query_items(query_items_list);
	}
}

void GEQODebugSpheres2D::_ready() {
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

	debug_draw = memnew(GEQODebugDraw2D);
	add_child(debug_draw);
	debug_draw->set_debug_color(debug_color);
}

void GEQODebugDraw2D::set_destroy_time(double time) {
	destroy_time = time;
}

void GEQODebugDraw2D::set_query_items(vector<QueryItem<Vector2>> &items) {
	saved_query_items = items;
	queue_redraw();
}
void GEQODebugDraw2D::set_debug_color(const Ref<Gradient> &gradient) {
	debug_color = gradient;
	queue_redraw();
}
void GEQODebugDraw2D::_draw() {
	Ref<Font> font = ThemeDB::get_singleton()->get_fallback_font();
	int32_t font_size = ThemeDB::get_singleton()->get_fallback_font_size();
	for (QueryItem<Vector2> &query_item : saved_query_items) {
		Color color;
		if (query_item.is_filtered) {
			color = Color(0, 0, 1);
		} else if (query_item.has_score) {
			color = debug_color->sample(query_item.score);
		} else {
			color = Color(0, 1, 1);
		}

		draw_rect(
				Rect2(
						query_item.projection_position - Vector2(16, 16),
						Vector2(32, 32)),
				color);

		if (query_item.is_filtered) {
			draw_string(font, query_item.projection_position, "Filtered", HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, color);
		} else if (query_item.has_score) {
			draw_string(font, query_item.projection_position, String::num(query_item.score), HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, Color(1, 1, 1, 0.8));
		} else {
			draw_string(font, query_item.projection_position, "Dolor", HORIZONTAL_ALIGNMENT_CENTER, -1, font_size, Color(1, 0, 0, 0.8));
		}
	}
}

void GEQODebugDraw2D::_bind_methods() {
}

void GEQODebugSpheres2D::_bind_methods() {
}