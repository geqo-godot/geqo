#ifndef ENVIRONMENTQUERY_H
#define ENVIRONMENTQUERY_H

#include <godot_cpp/classes/node.hpp>
#include <vector>
using std::vector;

namespace godot
{
    struct CQueryItem
    {
        double score = 0.0;
        bool is_filtered = false;
        bool has_score = false;
        Vector3 projection_position;
        Node *collided_with;
    };
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

}
#endif