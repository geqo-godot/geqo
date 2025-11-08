#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include "query_result.h"

namespace godot
{
    enum RaycastMode
    {
        BODY = 0,
        AREA = 1,
        BODY_AREA = 2
    };
    class CQueryGenerator3D : public Node3D
    {
        GDCLASS(CQueryGenerator3D, Node)

    protected:
        static void _bind_methods();

    public:
        CQueryGenerator3D();
        ~CQueryGenerator3D();

        virtual void perform_generation(vector<CQueryItem>);
        void perform_tests(vector<CQueryItem>);

    private:
        RaycastMode raycast_mode = BODY;
    };

}