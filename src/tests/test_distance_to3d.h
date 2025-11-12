#pragma once
#include "query_test3d.h"
#include "query_context3d.h"
#include <godot_cpp/classes/curve.hpp>

namespace godot
{
    class CTestDistanceTo : public CQueryTest3D
    {
        GDCLASS(CTestDistanceTo, CQueryTest3D)

    protected:
        static void _bind_methods();

    public:
        CTestDistanceTo();
        ~CTestDistanceTo();

        Ref<Curve> get_scoring_curve() const;
        void set_scoring_curve(Ref<Curve> curve);

        CQueryContext3D *get_distance_to() const;
        void set_distance_to(CQueryContext3D *context_node);

        double get_min_distance() const;
        void set_min_distance(double dist);

        double get_max_distance() const;
        void set_max_distance(double dist);

        void perform_test(CQueryItem &projection);
        void _ready() override;

    private:
        Ref<Curve> scoring_curve;
        CQueryContext3D *distance_to;
        double min_distance = 0.0;
        double max_distance = 10.0;
    };

}