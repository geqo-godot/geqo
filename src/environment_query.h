#ifndef ENVIRONMENTQUERY_H
#define ENVIRONMENTQUERY_H
struct CQueryItem
{
    double score;
    bool is_filtered;
    bool has_score;
    Vector3 projection_position;
    Node collided_with;
};

#include <godot_cpp/classes/node.hpp>
#include <vector>
using std::vector;
using namespace godot;

class CEnvironmentQuery : public Node
{
    GDCLASS(CEnvironmentQuery, Node)

protected:
    static void _bind_methods();

public:
    CEnvironmentQuery();
    ~CEnvironmentQuery();

    void set_use_debug_shapes(const bool use_debug);
    bool get_use_debug_shapes() const;

    void request_query();

private:
    bool use_debug_shapes;
    vector<CQueryItem> query_items;
};

#endif