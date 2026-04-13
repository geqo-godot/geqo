#pragma once
#include "query_enums.h"
#include <algorithm>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
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
public:
	using TestPurpose = GEQOEnums::TestPurpose;
	using TestType = GEQOEnums::TestType;
	using FilterType = GEQOEnums::FilterType;
	using MultipleContextScoreOp = GEQOEnums::MultipleContextScoreOp;
	using MultipleContextFilterOp = GEQOEnums::MultipleContextFilterOp;

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

	void _add_score_numeric(int test_purpose, int filter_type, double amount, double min_threshold, double max_threshold) {
		if (test_purpose == GEQOEnums::FILTER_SCORE || test_purpose == GEQOEnums::FILTER_ONLY) {
			bool filtered = false;
			if (filter_type == GEQOEnums::FILTER_TYPE_MIN)
				filtered = amount < min_threshold;
			else if (filter_type == GEQOEnums::FILTER_TYPE_MAX)
				filtered = amount > max_threshold;
			else if (filter_type == GEQOEnums::FILTER_TYPE_RANGE)
				filtered = amount < min_threshold || amount > max_threshold;
			if (filtered) {
				is_filtered = true;
				return;
			}
		}

		if (test_purpose == GEQOEnums::FILTER_SCORE || test_purpose == GEQOEnums::SCORE_ONLY) {
			double range = max_threshold - min_threshold;
			double normalized = (range > 0.0) ? std::clamp((amount - min_threshold) / range, 0.0, 1.0) : 0.0;
			score += normalized;
			has_score = true;
		}
	}

	void _add_score_boolean(int test_purpose, bool value, bool expected_boolean) {
		bool passed = (value == expected_boolean);

		if (test_purpose == 0 || test_purpose == 1) {
			if (!passed) {
				is_filtered = true;
				return;
			}
		}

		if (test_purpose == 0 || test_purpose == 2) {
			score += passed ? 1.0 : 0.0;
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

	void add_score_numeric(TestPurpose test_purpose, FilterType filter_type, double amount, double min_thershold, double max_threshold) {
		return _add_score_numeric(test_purpose, filter_type, amount, min_thershold, max_threshold);
	}
	void add_score_boolean(TestPurpose test_purpose, bool value, bool expected_boolean) {
		return _add_score_boolean(test_purpose, value, expected_boolean);
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
	using MultipleContextFilterOp = GEQOEnums::MultipleContextFilterOp;

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

	void add_score_numeric(TestPurpose test_purpose, FilterType filter_type, double amount, double min_thershold, double max_threshold) {
		return _add_score_numeric(test_purpose, filter_type, amount, min_thershold, max_threshold);
	}
	void add_score_boolean(TestPurpose test_purpose, bool value, bool expected_boolean) {
		return _add_score_boolean(test_purpose, value, expected_boolean);
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