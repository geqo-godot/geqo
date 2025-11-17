#pragma once
#include "query_result.h"
#include <godot_cpp/classes/node.hpp>
#include <debug/geqo_debug_spheres.h>
#include <vector>
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

    void _ready() override;

    Ref<CQueryResult> request_query();

private:
    bool use_debug_shapes = false;
    std::vector<CQueryItem> query_items = {};
    CGEQODebugSpheres *debug_spheres = nullptr;
};
