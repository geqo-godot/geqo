#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include "environment_query.h"
#include <godot_cpp/classes/node.hpp>
#include <vector>
using std::vector;
using namespace godot;

class CQueryResult : public RefCounted
{
    GDCLASS(CQueryResult, RefCounted)

protected:
    static void _bind_methods();

public:
    CQueryResult();
    ~CQueryResult();

    Vector3 get_highest_score_position() const;
    Node get_highest_score_node() const;

private:
    vector<CQueryItem> query_items;
};

#endif
