using Godot;


///<summary>
/// Tests each QueryItem3D from a QueryGenerator3D to determine which is the best option.
/// Named the same as native QueryItem3D so the EnvironmentQuery is able to call it.
///</summary> 
[Tool]
public partial class QueryTest3D : Node3D
{
    public enum TestPurposeEnum { FilterScore = 0, FilterOnly = 1, ScoreOnly = 2 }
    public enum TestTypeEnum { Numeric = 0, Boolean = 1 }
    public enum FilterTypeEnum { Min = 0, Max = 1, Range = 2 }
    public enum MultipleContextScoreOp { AverageScore = 0, MaxScore = 1, MinScore = 2 }
    public enum MultipleContextFilterOp { AnyPass = 0, AllPass = 1 }
    public enum ScoreClampType { None = 0, Value = 1, SameAsFilter = 2 }

    public float Cost
    {
        get => (float)Call("get_cost");
        set => Call("set_cost", value);
    }
    public FilterTypeEnum FilterType
    {
        get => (FilterTypeEnum)(int)Call("get_filter_type");
        set => Call("set_filter_type", (int)value);
    }

    public bool BoolMatch
    {
        get => (bool)Call("get_bool_match");
        set => Call("set_bool_match", value);
    }

    public float FilterMin
    {
        get => (float)Call("get_filter_min");
        set => Call("set_filter_min", value);
    }

    public float FilterMax
    {
        get => (float)Call("get_filter_max");
        set => Call("set_filter_max", value);
    }

    public MultipleContextFilterOp MultipleContextFilterOperator
    {
        get => (MultipleContextFilterOp)(int)Call("get_multiple_context_filter_operator");
        set => Call("set_multiple_context_filter_operator", (int)value);
    }

    public MultipleContextScoreOp MultipleContextScoreOperator
    {
        get => (MultipleContextScoreOp)(int)Call("get_multiple_context_score_operator");
        set => Call("set_multiple_context_score_operator", (int)value);
    }

    public Curve ScoreCurve
    {
        get => (Curve)(GodotObject)Call("get_scoring_curve");
        set => Call("set_scoring_curve", value);
    }

    public ScoreClampType ScoreClampMinType
    {
        get => (ScoreClampType)(int)Call("get_clamp_min_type");
        set => Call("set_clamp_min_type", (int)value);
    }

    public float ScoreClampMin
    {
        get => (float)Call("get_score_clamp_min");
        set => Call("set_score_clamp_min", value);
    }

    public ScoreClampType ScoreClampMaxType
    {
        get => (ScoreClampType)(int)Call("get_clamp_max_type");
        set => Call("set_clamp_max_type", (int)value);
    }

    public float ScoreClampMax
    {
        get => (float)Call("get_score_clamp_max");
        set => Call("set_score_clamp_max", value);
    }

    public float ScoringFactor
    {
        get => (float)Call("get_scoring_factor");
        set => Call("set_scoring_factor", value);
    }

    public TestPurposeEnum TestPurpose
    {
        get => (TestPurposeEnum)(int)Call("get_test_purpose");
        set => Call("set_test_purpose", (int)value);
    }

    public TestTypeEnum TestType
    {
        get => (TestTypeEnum)(int)Call("get_test_type");
        set => Call("set_test_type", (int)value);
    }
    /// <summary>
    /// The GDExtension calls this function
    /// </summary>
    public void _perform_test(RefCounted queryInstance)
    {
        QueryInstanceWrapper3D instance = new QueryInstanceWrapper3D(queryInstance);
        _PerformTest(instance);
    }

    public virtual void _PerformTest(QueryInstanceWrapper3D queryInstance)
    {
        GD.PrintErr("Need to override PerformTest in derived class");
    }

    public void EndTest()
    {
        Call("end_test");
    }
}