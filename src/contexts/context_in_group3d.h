#pragma once
#include "query_context3d.h"
using namespace godot;

class CContextInGroup3D : public CQueryContext3D
{
    GDCLASS(CContextInGroup3D, CQueryContext3D)

protected:
    static void _bind_methods();

public:
    CContextInGroup3D();
    ~CContextInGroup3D();

    void set_group(String new_group);
    String get_group();

    Array get_context() override;

private:
    String group = "";
};
