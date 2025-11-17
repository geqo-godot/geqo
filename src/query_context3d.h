#pragma once
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
class CQueryContext3D : public Node3D
{
    GDCLASS(CQueryContext3D, Node3D)

protected:
    static void _bind_methods();

public:
    CQueryContext3D();
    ~CQueryContext3D();

    virtual Array get_context() = 0;
    PackedVector3Array get_context_positions();

private:
};