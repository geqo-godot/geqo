#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include <algorithm>
#include <numeric>
#include <vector>

using namespace godot;

class QueryItem2D;
class QueryItem3D;

struct QueryItemTraits2D {
	using QueryItemT = QueryItem2D;
	using VectorT = Vector2;
	using NodeT = Node2D;
};

struct QueryItemTraits3D {
	using QueryItemT = QueryItem3D;
	using VectorT = Vector3;
	using NodeT = Node3D;
};

template <typename Traits>
class QueryItemBase {
protected:
	using QueryItemT = typename Traits::QueryItemT;
	using VectorT = typename Traits::VectorT;
	using NodeT = typename Traits::NodeT;
	double score = 0.0;
	bool is_filtered = false;
	bool has_score = false;
	VectorT projection_position = VectorT();
	NodeT *collided_with = nullptr;

public:
	QueryItemBase() = default;
	QueryItemBase(VectorT pos, NodeT *collider = nullptr) {
		projection_position = pos;
		collided_with = collider;
	}

	double _get_score() { return score; }
	void _set_score(double new_score) { score = new_score; }

	bool _get_is_filtered() { return is_filtered; }
	void _set_is_filtered(bool filtered) { is_filtered = filtered; }

	bool _get_has_score() { return has_score; }
	void _set_has_score(bool has) { has_score = has; }

	VectorT _get_projection_position() { return projection_position; }
	void _set_projection_position(VectorT position) { projection_position = position; }

	NodeT *_get_collided_with() { return collided_with; }
	void _set_collided_with(NodeT *node) { collided_with = node; }

	void _add_score(int test_purpose, int filter_operator, double amount, double min_thershold, double max_threshold) {
		// Filter out scores that aren't within the thresholds
		if (test_purpose == 0 || test_purpose == 1) {
			if (amount < min_thershold || amount > max_threshold) {
				is_filtered = true;
				return;
			}
		}

		// Handle scoring
		if (test_purpose == 0 || test_purpose == 2) {
			// Normalize the amount base on the thresholds
			double range = max_threshold - min_thershold;
			double normalized_score = (range > 0.0) ? std::clamp((amount - min_thershold) / range, 0.0, 1.0) : 0.0;
			score += normalized_score;
			has_score = true;
		}
	}

	bool is_higher_than(Ref<QueryItemT> item) {
		// Non filtered items come before filtered items
		if (is_filtered != item->is_filtered)
			return !is_filtered;

		// Items with score also come first
		if (has_score != item->has_score)
			return has_score;

		// Both have no score so do nothing
		if (!has_score)
			return false;

		return score > item->score;
	}
};

class QueryItem2D : public RefCounted, public QueryItemBase<QueryItemTraits2D> {
	GDCLASS(QueryItem2D, RefCounted)
public:
	double get_score() { return _get_score(); }
	void set_score(double new_score) { return _set_score(new_score); }

	bool get_is_filtered() { return _get_is_filtered(); }
	void set_is_filtered(bool filtered) { return _set_is_filtered(filtered); }

	bool get_has_score() { return _get_has_score(); }
	void set_has_score(bool has) { return _set_has_score(has); }

	Vector2 get_projection_position() { return _get_projection_position(); }
	void set_projection_position(Vector2 position) { return _set_projection_position(position); }

	Node2D *get_collided_with() { return _get_collided_with(); }
	void set_collided_with(Node2D *node) { return _set_collided_with(node); }

	void add_score(int test_purpose, int filter_operator, double amount, double min_thershold, double max_threshold) {
		return _add_score(test_purpose, filter_operator, amount, min_thershold, max_threshold);
	}

	// Factory thingy
	static Ref<QueryItem2D> create(Vector2 pos, Node2D *collider = nullptr) {
		Ref<QueryItem2D> item;
		item.instantiate();
		item->set_projection_position(pos);
		item->set_collided_with(collider);
		return item;
	}

protected:
	static void _bind_methods();
};

class QueryItem3D : public RefCounted, public QueryItemBase<QueryItemTraits3D> {
	GDCLASS(QueryItem3D, RefCounted)

public:
	double get_score() { return _get_score(); }
	void set_score(double new_score) { return _set_score(new_score); }

	bool get_is_filtered() { return _get_is_filtered(); }
	void set_is_filtered(bool filtered) { return _set_is_filtered(filtered); }

	bool get_has_score() { return _get_has_score(); }
	void set_has_score(bool has) { return _set_has_score(has); }

	Vector3 get_projection_position() { return _get_projection_position(); }
	void set_projection_position(Vector3 position) { return _set_projection_position(position); }

	Node3D *get_collided_with() { return _get_collided_with(); }
	void set_collided_with(Node3D *node) { return _set_collided_with(node); }

	void add_score(int test_purpose, int filter_operator, double amount, double min_thershold, double max_threshold) {
		return _add_score(test_purpose, filter_operator, amount, min_thershold, max_threshold);
	}

	// Factory thingy
	static Ref<QueryItem3D> create(Vector3 pos, Node3D *collider = nullptr) {
		Ref<QueryItem3D> item;
		item.instantiate();
		item->set_projection_position(pos);
		item->set_collided_with(collider);
		return item;
	}

protected:
	static void _bind_methods();
};