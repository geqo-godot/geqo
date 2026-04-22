using Godot;
public partial class QueryItemWrapper3D(RefCounted refCounted) : RefCounted
{
    private readonly RefCounted refCounted = refCounted;

    public enum TestPurposeEnum { FilterScore = 0, FilterOnly = 1, ScoreOnly = 2 }
    public enum TestTypeEnum { Numeric = 0, Boolean = 1 }
    public enum FilterTypeEnum { Min = 0, Max = 1, Range = 2 }
    public enum MultipleContextScoreOp { AverageScore = 0, MaxScore = 1, MinScore = 2 }
    public enum MultipleContextFilterOp { AnyPass = 0, AllPass = 1 }
    public enum ScoreClampType { None = 0, Value = 1, SameAsFilter = 2 }

    public RefCounted RawQueryItem => refCounted;
    public Node3D CollidedWith
    {
        get => (Node3D)(GodotObject)refCounted.Call("get_collided_with");
        set => refCounted.Call("set_collided_with", value);
    }
    public bool IsFiltered
    {
        get => (bool)refCounted.Call("get_is_filtered");
        set => refCounted.Call("set_is_filtered", value);
    }
    public Vector3 ProjectionPosition
    {
        get => (Vector3)refCounted.Call("get_projection_position");
        set => refCounted.Call("set_projection_position", value);
    }
    public float Score
    {
        get => (float)refCounted.Call("get_score");
        set => refCounted.Call("set_score", value);
    }

    public void AddScoreBoolean(TestPurposeEnum testPurpose, bool value, bool expectedBoolean)
    {
        refCounted.Call(Methods.AddScoreBoolean, (int)testPurpose, value, expectedBoolean);
    }

    public void AddScoreDirect(TestPurposeEnum testPurpose, float normalizedValue, float scoringFactor)
    {
        refCounted.Call(Methods.AddScoreDirect, (int)testPurpose, normalizedValue, scoringFactor);
    }

    public void AddScoreNumeric(TestPurposeEnum testPurpose, FilterTypeEnum filterType, float amount, float minThreshold, float maxThreshold)
    {
        refCounted.Call(Methods.AddScoreNumeric, (int)testPurpose, (int)filterType, amount, minThreshold, maxThreshold);
    }

    public void ApplyFilterBoolean(bool value, bool expected)
    {
        refCounted.Call(Methods.ApplyFilterBoolean, value, expected);
    }

    public void ApplyFilterNumeric(FilterTypeEnum filterType, float amount, float minThreshold, float maxThreshold)
    {
        refCounted.Call(Methods.ApplyFilterNumeric, (int)filterType, amount, minThreshold, maxThreshold);
    }

    public static QueryItemWrapper3D Create(Vector3 position, Node3D collider)
    {
        RefCounted new_item = (RefCounted)(GodotObject)ClassDB.Instantiate("QueryItem3D");
        new_item.Call("set_projection_position", position);
        new_item.Call("set_collided_with", collider);
        return new QueryItemWrapper3D(new_item);
    }

    private static class Methods
    {
        public static readonly StringName AddScoreBoolean = "add_score_boolean";
        public static readonly StringName AddScoreDirect = "add_score_direct";
        public static readonly StringName AddScoreNumeric = "add_score_numeric";
        public static readonly StringName ApplyFilterBoolean = "apply_filter_boolean";
        public static readonly StringName ApplyFilterNumeric = "apply_filter_numeric";
        public static readonly StringName Create = "create";
    }
}
