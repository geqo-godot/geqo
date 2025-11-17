#pragma once
#include "query_test3d.h"
#include "query_context3d.h"
using namespace godot;
class CTestDotProduct3D : public CQueryTest3D
{
    GDCLASS(CTestDotProduct3D, CQueryTest3D)

protected:
    static void _bind_methods();

public:
    CTestDotProduct3D();
    ~CTestDotProduct3D();

    double get_min_value();
    void set_min_value(double value);

    double get_max_value();
    void set_max_value(double value);

    CQueryContext3D *get_from_place();
    void set_from_place(CQueryContext3D *context);

    CQueryContext3D *get_to_target();
    void set_to_target(CQueryContext3D *context);

    void perform_test(CQueryItem &projection) override;

private:
    double min_value = 0.5;
    double max_value = 1.0;

    CQueryContext3D *from_place = nullptr;
    CQueryContext3D *to_target = nullptr;
};