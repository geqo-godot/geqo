#include "query_result.h"
#include <godot_cpp/core/class_db.hpp>
#include <algorithm>

using namespace godot;

void CQueryResult::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_highest_score_position"), &CQueryResult::get_highest_score_position);
    ClassDB::bind_method(D_METHOD("get_highest_score_node"), &CQueryResult::get_highest_score_node);
}

CQueryResult::CQueryResult()
{
}

CQueryResult::~CQueryResult()
{
    // Add your cleanup here.
}

Vector3 CQueryResult::get_highest_score_position() const
{
    if (query_items.empty())
        return Vector3();

    std::vector<CQueryItem>::const_iterator best_score = std::max_element(query_items.begin(), query_items.end());

    return best_score->projection_position;
}

Node *CQueryResult::get_highest_score_node() const
{
    if (query_items.empty())
        return nullptr;

    std::vector<CQueryItem>::const_iterator best_score = std::max_element(query_items.begin(), query_items.end());
    return best_score->collided_with;
}

std::vector<CQueryItem> &CQueryResult::get_query_items()
{
    return query_items;
}
