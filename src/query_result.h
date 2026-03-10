#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <vector>
using namespace godot;

template <typename QueryItemT, typename VectorT, typename NodeT>
class QueryItemBase {
protected:
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

	void _add_score(double amount) {
		score += amount;
		has_score = true;
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

class QueryItem2D : public RefCounted, public QueryItemBase<QueryItem2D, Vector2, Node2D> {
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

	void add_score(double amount) { return _add_score(amount); }

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

class QueryItem3D : public RefCounted, public QueryItemBase<QueryItem3D, Vector3, Node3D> {
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

	void add_score(double amount) { return _add_score(amount); }

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
template <typename VectorT, typename QueryItemT, typename NodeT>
class QueryResultBase {
private:
	// Query Items of result, EnvironmentQuery should tranfer ownership to it
	std::vector<Ref<QueryItemT>> query_items;
	mutable bool is_cache_built = false;
	mutable std::vector<size_t> sorted_indices;
	mutable int highest_unfiltered_index = -1;

public:
	~QueryResultBase() = default;

	void set_items(const std::vector<Ref<QueryItemT>> &items) { query_items = items; }
	void _build_cache() const;

	TypedArray<VectorT> _get_all_position() const;
	TypedArray<NodeT> _get_all_node() const;

	VectorT _get_highest_score_position() const;
	VectorT _get_top_random_position(double percent = 0.1) const;
	NodeT *_get_highest_score_node() const;
	NodeT *_get_top_random_node(double percent = 0.1) const;
};

class QueryResult2D : public RefCounted, public QueryResultBase<Vector2, QueryItem2D, Node2D> {
	GDCLASS(QueryResult2D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Vector2> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector2 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector2 get_top_random_position(double percent = 0.05) const { return _get_top_random_position(percent); }
	Node2D *get_highest_score_node() const { return _get_highest_score_node(); };
	Node2D *get_top_random_node(double percent = 0.05) const { return _get_top_random_node(percent); }
};

class QueryResult3D : public RefCounted, public QueryResultBase<Vector3, QueryItem3D, Node3D> {
	GDCLASS(QueryResult3D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Vector3> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector3 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector3 get_top_random_position(double percent = 0.1) const { return _get_top_random_position(percent); }
	Node3D *get_highest_score_node() const { return _get_highest_score_node(); };
	Node3D *get_top_random_node(double percent = 0.1) const { return _get_top_random_node(percent); }
};
