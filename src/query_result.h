#pragma once
#include <godot_cpp/classes/node.hpp>
#include <vector>
using namespace godot;

struct CQueryItem
{
    double score = 0.0;
    bool is_filtered = false;
    bool has_score = false;
    Vector3 projection_position = Vector3(0.0, 0.0, 0.0);
    Node *collided_with = nullptr;

    CQueryItem(Vector3 pos)
    {
        projection_position = pos;
    }

    void add_score(double amount)
    {
        score += amount;
        has_score = true;
    }

    bool operator<(const CQueryItem &item) const
    {
        if (is_filtered || !has_score)
            return true;
        return score < item.score;
    }

    CQueryItem &operator=(const CQueryItem &item)
    {
        score = item.score;
        is_filtered = item.is_filtered;
        has_score = item.has_score;
        projection_position = item.projection_position;
        collided_with = item.collided_with;
        return *this;
    }
};
class CQueryResult : public RefCounted
{
    GDCLASS(CQueryResult, RefCounted)
private:
    std::vector<CQueryItem> query_items;

protected:
    static void _bind_methods();

public:
    CQueryResult();
    ~CQueryResult();

    void set_items(const std::vector<CQueryItem> &items) { query_items = items; }

    Vector3 get_highest_score_position() const;
    Node *get_highest_score_node() const;
    std::vector<CQueryItem> &get_query_items();
};