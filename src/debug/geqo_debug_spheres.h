#pragma once
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

template <typename VectorT>
class GEQODebugSpheresBase {
protected:
	Array text_labels;
	Ref<Gradient> debug_color;

public:
	~GEQODebugSpheresBase() = default;

	virtual void draw_items(vector<QueryItem<VectorT>> &query_items_list, double time_to_destroy = 2.0) = 0;
	void remove_labels() {
		for (const Variant &label : text_labels) {
			Node *label_ref = Object::cast_to<Node>(label);
			if (label_ref == nullptr) {
				print_error("Not a label");
				continue;
			}
			label_ref->queue_free();
		}
		text_labels.clear();
	}
};