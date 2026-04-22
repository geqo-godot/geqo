using Godot;


///<summary>
/// Tests each QueryItem2D from a QueryGenerator2D to determine which is the best option.
/// Named the same as native QueryItem2D so the EnvironmentQuery is able to call it.
///</summary> 
[Tool]
public partial class QueryTest2D : Node2D
{
    public float Cost
    {
        get => (float)Call("get_cost");
        set => Call("set_cost", value);
    }
    public GEQOEnums.FilterType FilterType
    {
        get => (GEQOEnums.FilterType)(int)Call("get_filter_type");
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

    public GEQOEnums.MultipleContextFilterOp MultipleContextFilterOperator
    {
        get => (GEQOEnums.MultipleContextFilterOp)(int)Call("get_multiple_context_filter_operator");
        set => Call("set_multiple_context_filter_operator", (int)value);
    }

    public GEQOEnums.MultipleContextScoreOp MultipleContextScoreOperator
    {
        get => (GEQOEnums.MultipleContextScoreOp)(int)Call("get_multiple_context_score_operator");
        set => Call("set_multiple_context_score_operator", (int)value);
    }

    public Curve ScoreCurve
    {
        get => (Curve)(GodotObject)Call("get_scoring_curve");
        set => Call("set_scoring_curve", value);
    }

    public GEQOEnums.ScoreClampType ScoreClampMinType
    {
        get => (GEQOEnums.ScoreClampType)(int)Call("get_clamp_min_type");
        set => Call("set_clamp_min_type", (int)value);
    }

    public float ScoreClampMin
    {
        get => (float)Call("get_score_clamp_min");
        set => Call("set_score_clamp_min", value);
    }

    public GEQOEnums.ScoreClampType ScoreClampMaxType
    {
        get => (GEQOEnums.ScoreClampType)(int)Call("get_clamp_max_type");
        set => Call("set_clamp_max_type", (int)value);
    }

    public float ScoreClampMax
    {
        get => (float)Call("get_score_clamp_max");
        set => Call("set_score_clamp_max", value);
    }

    public float ScoreFactor
    {
        get => (float)Call("get_scoring_factor");
        set => Call("set_scoring_factor", value);
    }

    public GEQOEnums.TestPurpose TestPurpose
    {
        get => (GEQOEnums.TestPurpose)(int)Call("get_test_purpose");
        set => Call("set_test_purpose", (int)value);
    }

    public GEQOEnums.TestType TestType
    {
        get => (GEQOEnums.TestType)(int)Call("get_test_type");
        set => Call("set_test_type", (int)value);
    }
    /// <summary>
    /// The GDExtension calls this function.
    /// </summary>
    void _perform_test(RefCounted queryInstance)
    {
        QueryInstanceWrapper2D instance = new QueryInstanceWrapper2D(queryInstance);
        _PerformTest(instance);
    }

    public virtual void _PerformTest(QueryInstanceWrapper2D queryInstance)
    {
        GD.PrintErr("Need to override PerformTest in derived class");
    }

    public void EndTest()
    {
        Call("end_test");
    }
}