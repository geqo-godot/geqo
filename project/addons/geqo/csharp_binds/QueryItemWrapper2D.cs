using Godot;
public partial class QueryItemWrapper2D(RefCounted refCounted) : RefCounted
{
    private readonly RefCounted refCounted = refCounted;

    public RefCounted RawQueryItem => refCounted;
    public Node2D CollidedWith
    {
        get => (Node2D)(GodotObject)refCounted.Call("get_collided_with");
        set => refCounted.Call("set_collided_with", value);
    }
    public bool IsFiltered
    {
        get => (bool)refCounted.Call("get_is_filtered");
        set => refCounted.Call("set_is_filtered", value);
    }
    public Vector2 ProjectionPosition
    {
        get => (Vector2)refCounted.Call("get_projection_position");
        set => refCounted.Call("set_projection_position", value);
    }
    public float Score
    {
        get => (float)refCounted.Call("get_score");
        set => refCounted.Call("set_score", value);
    }

    public void AddScoreBoolean(GEQOEnums.TestPurpose testPurpose, bool value, bool expectedBoolean)
    {
        refCounted.Call(Methods.AddScoreBoolean, (int)testPurpose, value, expectedBoolean);
    }

    public void AddScoreDirect(GEQOEnums.TestPurpose testPurpose, float normalizedValue, float scoringFactor)
    {
        refCounted.Call(Methods.AddScoreDirect, (int)testPurpose, normalizedValue, scoringFactor);
    }

    public void AddScoreNumeric(GEQOEnums.TestPurpose testPurpose, GEQOEnums.FilterType filterType, float amount, float minThreshold, float maxThreshold)
    {
        refCounted.Call(Methods.AddScoreNumeric, (int)testPurpose, (int)filterType, amount, minThreshold, maxThreshold);
    }

    public bool ApplyFilterBoolean(bool value, bool expected)
    {
        return (bool)refCounted.Call(Methods.ApplyFilterBoolean, value, expected);
    }

    public bool ApplyFilterNumeric(GEQOEnums.FilterType filterType, float amount, float minThreshold, float maxThreshold)
    {
        return (bool)refCounted.Call(Methods.ApplyFilterNumeric, (int)filterType, amount, minThreshold, maxThreshold);
    }

    public static QueryItemWrapper2D Create(Vector2 position, Node2D collider)
    {
        RefCounted new_item = (RefCounted)(GodotObject)ClassDB.Instantiate("QueryItem2D");
        new_item.Call("set_projection_position", position);
        new_item.Call("set_collided_with", collider);
        return new QueryItemWrapper2D(new_item);
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
