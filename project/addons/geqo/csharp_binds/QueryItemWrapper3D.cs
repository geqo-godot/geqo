using Godot;
public partial class QueryItemWrapper3D(RefCounted refCounted) : RefCounted
{
    private readonly RefCounted refCounted = refCounted;

    public RefCounted RawQueryItem => refCounted;
    public Node3D CollidedWith
    {
        get => (Node3D)(GodotObject)refCounted.Call(Methods.GetCollidedWith);
        set => refCounted.Call(Methods.SetCollidedWith, value);
    }
    public bool IsFiltered
    {
        get => (bool)refCounted.Call(Methods.GetIsFiltered);
        set => refCounted.Call(Methods.SetIsFiltered, value);
    }
    public Vector3 ProjectionPosition
    {
        get => (Vector3)refCounted.Call(Methods.GetProjectionPosition);
        set => refCounted.Call(Methods.SetProjectionPosition, value);
    }
    public float Score
    {
        get => (float)refCounted.Call(Methods.GetScore);
        set => refCounted.Call(Methods.SetScore, value);
    }

    public void AddScoreBoolean(GEQOEnums.TestPurpose testPurpose, bool value, bool expectedBoolean) => refCounted.Call(Methods.AddScoreBoolean, (int)testPurpose, value, expectedBoolean);

    public void AddScoreDirect(GEQOEnums.TestPurpose testPurpose, float normalizedValue, float scoringFactor) => refCounted.Call(Methods.AddScoreDirect, (int)testPurpose, normalizedValue, scoringFactor);

    public void AddScoreNumeric(GEQOEnums.TestPurpose testPurpose, GEQOEnums.FilterType filterType, float amount, float minThreshold, float maxThreshold) => refCounted.Call(Methods.AddScoreNumeric, (int)testPurpose, (int)filterType, amount, minThreshold, maxThreshold);

    public bool ApplyFilterBoolean(bool value, bool expected) => (bool)refCounted.Call(Methods.ApplyFilterBoolean, value, expected);

    public bool ApplyFilterNumeric(GEQOEnums.FilterType filterType, float amount, float minThreshold, float maxThreshold) => (bool)refCounted.Call(Methods.ApplyFilterNumeric, (int)filterType, amount, minThreshold, maxThreshold);

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

	public static readonly StringName GetCollidedWith = "get_collided_with";
	public static readonly StringName SetCollidedWith = "set_collided_with";
	public static readonly StringName GetIsFiltered = "get_is_filtered";
	public static readonly StringName SetIsFiltered = "set_is_filtered";
	public static readonly StringName GetProjectionPosition = "get_projection_position";
	public static readonly StringName SetProjectionPosition = "set_projection_position";
	public static readonly StringName GetScore = "get_score";
	public static readonly StringName SetScore = "set_score";
    }
}
