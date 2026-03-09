
#pragma once
#include "debug/geqo_debug_spheres.h"
#include "query_result.h"
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class GEQODebugDraw2D : public Node2D {
	GDCLASS(GEQODebugDraw2D, Node2D)
private:
	std::vector<Ref<QueryItem2D>> saved_query_items;
	Ref<Gradient> debug_color;
	double destroy_time = 0.0;

public:
	GEQODebugDraw2D() {}
	~GEQODebugDraw2D() {}

	void set_destroy_time(double time);
	void set_query_items(std::vector<Ref<QueryItem2D>> &items);
	void set_debug_color(const Ref<Gradient> &gradient);
	void _draw() override;

protected:
	static void _bind_methods();
};

class GEQODebugSpheres2D : public Node, public GEQODebugSpheresBase<Vector2, QueryItem2D> {
	GDCLASS(GEQODebugSpheres2D, Node)

private:
	GEQODebugDraw2D *debug_draw = nullptr;
	std::vector<Ref<QueryItem2D>> saved_query_items;

public:
	GEQODebugSpheres2D() {}
	~GEQODebugSpheres2D() {}

	void draw_items(std::vector<Ref<QueryItem2D>> &query_items_list, double time_to_destroy = 2.0);

	void _ready() override;

protected:
	static void _bind_methods();
};