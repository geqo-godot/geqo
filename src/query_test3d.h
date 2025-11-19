#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include "query_result.h"
using namespace godot;
class CQueryTest3D : public Node3D
{
    GDCLASS(CQueryTest3D, Node3D)
protected:
    static void _bind_methods();

public:
    enum TestPurpose : unsigned int
    {
        FILTER_SCORE,
        FILTER_ONLY,
        SCORE_ONLY,
    };
    enum ScoreOperator : unsigned int
    {
        AVERAGE_SCORE,
        MAX_SCORE,
        MIN_SCORE,
    };
    CQueryTest3D();
    ~CQueryTest3D();

    TestPurpose get_test_purpose() const;
    void set_test_purpose(const TestPurpose purpose);
    ScoreOperator get_context_operator() const;
    void set_context_operator(const ScoreOperator score_op);

    virtual void perform_test(CQueryItem &projection) = 0;

private:
    TestPurpose test_purpose = FILTER_SCORE;
    ScoreOperator multiple_context_operator = AVERAGE_SCORE;
};

VARIANT_ENUM_CAST(CQueryTest3D::TestPurpose);
VARIANT_ENUM_CAST(CQueryTest3D::ScoreOperator);
