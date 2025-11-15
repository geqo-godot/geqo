#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include "query_result.h"

namespace godot
{
    class CQueryGenerator3D : public Node3D
    {
        GDCLASS(CQueryGenerator3D, Node)

    protected:
        static void _bind_methods();

    public:
        enum RaycastMode
        {
            BODY,
            AREA,
            BODY_AREA
        };
        CQueryGenerator3D();
        ~CQueryGenerator3D();

        RaycastMode get_raycast_mode() const;
        void set_raycast_mode(RaycastMode mode);

        virtual void perform_generation(vector<CQueryItem> &query_item_list) = 0;
        void perform_tests(vector<CQueryItem> &query_item_list);

    private:
        RaycastMode raycast_mode = BODY;
    };

}
VARIANT_ENUM_CAST(CQueryGenerator3D::RaycastMode);