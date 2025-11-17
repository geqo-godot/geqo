#pragma once
#include "query_test3d.h"
#include "query_context3d.h"
#include <godot_cpp/classes/curve.hpp>
using namespace godot;
class CTestDistanceTo3D : public CQueryTest3D
{
    GDCLASS(CTestDistanceTo3D, CQueryTest3D)

protected:
    static void _bind_methods();

public:
    CTestDistanceTo3D();
    ~CTestDistanceTo3D();

    Ref<Curve> get_scoring_curve() const;
    void set_scoring_curve(Ref<Curve> curve);

    CQueryContext3D *get_distance_to();
    void set_distance_to(CQueryContext3D *context_node);

    double get_min_distance() const;
    void set_min_distance(double dist);

    double get_max_distance() const;
    void set_max_distance(double dist);

    void perform_test(CQueryItem &projection) override;
    void _ready() override;

private:
    Ref<Curve> scoring_curve;
    CQueryContext3D *distance_to = nullptr;

    double min_distance = 0.0;
    double max_distance = 10.0;
};