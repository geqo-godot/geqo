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
        get => (float)Call(Methods.GetCost);
        set => Call(Methods.SetCost, value);
    }
    public GEQOEnums.FilterType FilterType
    {
        get => (GEQOEnums.FilterType)(int)Call(Methods.GetFilterType);
        set => Call(Methods.SetFilterType, (int)value);
    }

    public bool BoolMatch
    {
        get => (bool)Call(Methods.GetBoolMatch);
        set => Call(Methods.SetBoolMatch, value);
    }

    public float FilterMin
    {
        get => (float)Call(Methods.GetFilterMin);
        set => Call(Methods.SetFilterMin, value);
    }

    public float FilterMax
    {
        get => (float)Call(Methods.GetFilterMax);
        set => Call(Methods.SetFilterMax, value);
    }

    public GEQOEnums.MultipleContextFilterOp MultipleContextFilterOperator
    {
        get => (GEQOEnums.MultipleContextFilterOp)(int)Call(Methods.GetMultipleContextFilterOperator);
        set => Call(Methods.SetMultipleContextFilterOperator, (int)value);
    }

    public GEQOEnums.MultipleContextScoreOp MultipleContextScoreOperator
    {
        get => (GEQOEnums.MultipleContextScoreOp)(int)Call(Methods.GetMultipleContextScoreOperator);
        set => Call(Methods.SetMultipleContextScoreOperator, (int)value);
    }

    public Curve ScoreCurve
    {
        get => (Curve)(GodotObject)Call(Methods.GetScoringCurve);
        set => Call(Methods.SetScoringCurve, value);
    }

    public GEQOEnums.ScoreClampType ScoreClampMinType
    {
        get => (GEQOEnums.ScoreClampType)(int)Call(Methods.GetClampMinType);
        set => Call(Methods.SetClampMinType, (int)value);
    }

    public float ScoreClampMin
    {
        get => (float)Call(Methods.GetScoreClampMin);
        set => Call(Methods.SetScoreClampMin, value);
    }

    public GEQOEnums.ScoreClampType ScoreClampMaxType
    {
        get => (GEQOEnums.ScoreClampType)(int)Call(Methods.GetClampMaxType);
        set => Call(Methods.SetClampMaxType, (int)value);
    }

    public float ScoreClampMax
    {
        get => (float)Call(Methods.GetScoreClampMax);
        set => Call(Methods.SetScoreClampMax, value);
    }

    public float ScoreFactor
    {
        get => (float)Call(Methods.GetScoringFactor);
        set => Call(Methods.SetScoringFactor, value);
    }

    public GEQOEnums.TestPurpose TestPurpose
    {
        get => (GEQOEnums.TestPurpose)(int)Call(Methods.GetTestPurpose);
        set => Call(Methods.SetTestPurpose, (int)value);
    }

    public GEQOEnums.TestType TestType
    {
        get => (GEQOEnums.TestType)(int)Call(Methods.GetTestType);
        set => Call(Methods.SetTestType, (int)value);
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

    public void EndTest() => Call("end_test");

    private static class Methods
    {
        public static readonly StringName GetCost = "get_cost";
        public static readonly StringName SetCost = "set_cost";
        public static readonly StringName GetFilterType = "get_filter_type";
        public static readonly StringName SetFilterType = "set_filter_type";
        public static readonly StringName GetBoolMatch = "get_bool_match";
        public static readonly StringName SetBoolMatch = "set_bool_match";
        public static readonly StringName GetFilterMin = "get_filter_min";
        public static readonly StringName SetFilterMin = "set_filter_min";
        public static readonly StringName GetFilterMax = "get_filter_max";
        public static readonly StringName SetFilterMax = "set_filter_max";
        public static readonly StringName GetMultipleContextFilterOperator = "get_multiple_context_filter_operator";
        public static readonly StringName SetMultipleContextFilterOperator = "set_multiple_context_filter_operator";
        public static readonly StringName GetMultipleContextScoreOperator = "get_multiple_context_score_operator";
        public static readonly StringName SetMultipleContextScoreOperator = "set_multiple_context_score_operator";
        public static readonly StringName GetScoringCurve = "get_scoring_curve";
        public static readonly StringName SetScoringCurve = "set_scoring_curve";
        public static readonly StringName GetClampMinType = "get_clamp_min_type";
        public static readonly StringName SetClampMinType = "set_clamp_min_type";
        public static readonly StringName GetScoreClampMin = "get_score_clamp_min";
        public static readonly StringName SetScoreClampMin = "set_score_clamp_min";
        public static readonly StringName GetClampMaxType = "get_clamp_max_type";
        public static readonly StringName SetClampMaxType = "set_clamp_max_type";
        public static readonly StringName GetScoreClampMax = "get_score_clamp_max";
        public static readonly StringName SetScoreClampMax = "set_score_clamp_max";
        public static readonly StringName GetScoringFactor = "get_scoring_factor";
        public static readonly StringName SetScoringFactor = "set_scoring_factor";
        public static readonly StringName GetTestPurpose = "get_test_purpose";
        public static readonly StringName SetTestPurpose = "set_test_purpose";
        public static readonly StringName GetTestType = "get_test_type";
        public static readonly StringName SetTestType = "set_test_type";
    }
}
