#ifndef ENVIRONMENTQUERY_H
#define ENVIRONMENTQUERY_H

#include <godot_cpp/classes/node.hpp>

namespace godot
{

    class CEnvironmentQuery : public Node
    {
        GDCLASS(CEnvironmentQuery, Node)

    private:
        double time_passed;

    protected:
        static void _bind_methods();

    public:
        CEnvironmentQuery();
        ~CEnvironmentQuery();

        void _process(double delta) override;
    };

}

#endif
