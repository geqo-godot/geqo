#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <numeric>

using namespace godot;

template <typename VectorT, typename QueryItemT, typename NodeT>
TypedArray<VectorT> QueryResultBase<VectorT, QueryItemT, NodeT>::_get_all_position() const {
	TypedArray<VectorT> result;

	_build_cache();

	for (int i = 0; i <= highest_unfiltered_index; i++) {
		result.append(query_items[sorted_indices[i]]->get_projection_position());
	}
	return result;
}

template <typename VectorT, typename QueryItemT, typename NodeT>
TypedArray<Ref<QueryItemT>> QueryResultBase<VectorT, QueryItemT, NodeT>::_get_all_results() const {
	TypedArray<QueryItemT> result;

	_build_cache();

	// Has to be turned into type array for godot
	for (int i = 0; i <= highest_unfiltered_index; i++) {
		result.append(query_items[sorted_indices[i]]);
	}
	return result;
}

template <typename VectorT, typename QueryItemT, typename NodeT>
TypedArray<NodeT> QueryResultBase<VectorT, QueryItemT, NodeT>::_get_all_node() const {
	TypedArray<NodeT> result;

	_build_cache();

	for (int i = 0; i <= highest_unfiltered_index; i++) {
		result.append(query_items[sorted_indices[i]]->get_collided_with());
	}
	return result;
}

template <typename VectorT, typename QueryItemT, typename NodeT>
VectorT QueryResultBase<VectorT, QueryItemT, NodeT>::_get_highest_score_position() const {
	if (query_items.empty())
		return VectorT();

	_build_cache();

	return query_items[sorted_indices[0]]->get_projection_position();
}

template <typename VectorT, typename QueryItemT, typename NodeT>
VectorT QueryResultBase<VectorT, QueryItemT, NodeT>::_get_top_random_position(double percent) const {
	if (query_items.empty())
		return VectorT();

	_build_cache();

	percent = std::clamp(percent, 0.0, 1.0);

	int unfiltered_count = highest_unfiltered_index + 1;
	// If there are no results
	if (unfiltered_count <= 0)
		return VectorT();

	// Get the top percentage
	int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
	top_count = std::max(top_count, 1);

	int random_i = UtilityFunctions::randi_range(0, top_count - 1);
	return query_items[sorted_indices[random_i]]->get_projection_position();
}

template <typename VectorT, typename QueryItemT, typename NodeT>
NodeT *QueryResultBase<VectorT, QueryItemT, NodeT>::_get_highest_score_node() const {
	if (query_items.empty())
		return nullptr;

	_build_cache();
	return query_items[sorted_indices[0]]->get_collided_with();
}
template <typename VectorT, typename QueryItemT, typename NodeT>
NodeT *QueryResultBase<VectorT, QueryItemT, NodeT>::_get_top_random_node(double percent) const {
	if (query_items.empty())
		return nullptr;

	_build_cache();

	percent = std::clamp(percent, 0.0, 1.0);

	int unfiltered_count = highest_unfiltered_index + 1;
	// If there are no results
	if (unfiltered_count <= 0)
		return nullptr;

	// Get the top percentage
	int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
	top_count = std::max(top_count, 1);

	int random_i = UtilityFunctions::randi_range(0, top_count - 1);
	return query_items[sorted_indices[random_i]]->get_collided_with();
}

// Sort indices and store them for future calls
template <typename VectorT, typename QueryItemT, typename NodeT>
void QueryResultBase<VectorT, QueryItemT, NodeT>::_build_cache() const {
	if (is_cache_built) {
		return;
	}

	sorted_indices.resize(query_items.size());
	// Init sorted_vertices
	std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

	// Sort in descending order
	std::sort(
			sorted_indices.begin(),
			sorted_indices.end(),
			[&](size_t lhs, size_t rhs) {
				return query_items[lhs]->is_higher_than(query_items[rhs]);
			});
	for (size_t i = 0; i < sorted_indices.size(); i++) {
		if (query_items[sorted_indices[i]]->get_is_filtered())
			break;
		else
			highest_unfiltered_index = i;
	}

	is_cache_built = true;
}

void QueryItem2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_score"), &QueryItem2D::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &QueryItem2D::get_score);
	ClassDB::bind_method(D_METHOD("set_is_filtered"), &QueryItem2D::set_is_filtered);
	ClassDB::bind_method(D_METHOD("get_is_filtered"), &QueryItem2D::get_is_filtered);
	ClassDB::bind_method(D_METHOD("set_projection_position"), &QueryItem2D::set_projection_position);
	ClassDB::bind_method(D_METHOD("get_projection_position"), &QueryItem2D::get_projection_position);
	ClassDB::bind_method(D_METHOD("set_collided_with"), &QueryItem2D::set_collided_with);
	ClassDB::bind_method(D_METHOD("get_collided_with"), &QueryItem2D::get_collided_with);
	ClassDB::bind_method(D_METHOD("add_score"), &QueryItem2D::add_score);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "set_score", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collided_with", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_collided_with", "get_collided_with");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_filtered"), "set_is_filtered", "get_is_filtered");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "projection_position"), "set_projection_position", "get_projection_position");

	ClassDB::bind_static_method("QueryItem2D", D_METHOD("create", "position", "collider"), &QueryItem2D::create);
}

void QueryItem3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_score"), &QueryItem3D::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &QueryItem3D::get_score);
	ClassDB::bind_method(D_METHOD("set_is_filtered"), &QueryItem3D::set_is_filtered);
	ClassDB::bind_method(D_METHOD("get_is_filtered"), &QueryItem3D::get_is_filtered);
	ClassDB::bind_method(D_METHOD("set_projection_position"), &QueryItem3D::set_projection_position);
	ClassDB::bind_method(D_METHOD("get_projection_position"), &QueryItem3D::get_projection_position);
	ClassDB::bind_method(D_METHOD("set_collided_with"), &QueryItem3D::set_collided_with);
	ClassDB::bind_method(D_METHOD("get_collided_with"), &QueryItem3D::get_collided_with);
	ClassDB::bind_method(D_METHOD("add_score"), &QueryItem3D::add_score);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "set_score", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collided_with", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_collided_with", "get_collided_with");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_filtered"), "set_is_filtered", "get_is_filtered");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "projection_position"), "set_projection_position", "get_projection_position");

	ClassDB::bind_static_method("QueryItem3D", D_METHOD("create", "position", "collider"), &QueryItem3D::create);
}

void QueryResult2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_results"), &QueryResult2D::get_all_results);
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult2D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult2D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult2D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult2D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult2D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult2D::get_top_random_node, DEFVAL(0.1));
}

void QueryResult3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_results"), &QueryResult3D::get_all_results);
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult3D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult3D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult3D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult3D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult3D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult3D::get_top_random_node, DEFVAL(0.1));
}
