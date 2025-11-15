#pragma once
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <debug/geqo_debug_spheres.h>
#include <vector>
using std::vector;

namespace godot
{
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

        void _ready() override;

        Ref<CQueryResult> request_query();

    private:
        bool use_debug_shapes;
        vector<CQueryItem> query_items;
        // TODO: Check how many rays have been casted in total, cap it so it does a certain amount of rays per tick
        // Since rays are the heaviest calculations by far I think
        int casted_rays = 0;
        int rays_per_tick = 1000;
        CGEQODebugSpheres *debug_spheres = nullptr;
    };

}