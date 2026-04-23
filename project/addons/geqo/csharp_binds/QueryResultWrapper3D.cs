using Godot;
/// <summary>
/// Wraps over the GDExtension QueryResult3D.
/// </summary>
using Godot.Collections;
public partial class QueryResultWrapper3D(RefCounted refCounted)
{
    private readonly RefCounted refCounted = refCounted;

    public Array<Node3D> GetAllNode() => (Array<Node3D>)refCounted.Call(MethodName.GetAllNode);

    public Vector3[] GetAllPosition() => (Vector3[])refCounted.Call(MethodName.GetAllPosition);

    public Array<QueryItemWrapper3D> GetAllResults()
    {
        var result = new Array<QueryItemWrapper3D>();
        foreach (RefCounted item in (Array)refCounted.Call(MethodName.GetAllResults))
            result.Add(new QueryItemWrapper3D(item));
        return result;
    }

    public Node3D GetHighestScoreNode() => (Node3D)(GodotObject)refCounted.Call(MethodName.GetHighestScoreNode);

    public Vector3 GetHighestScorePosition() => (Vector3)refCounted.Call(MethodName.GetHighestScorePosition);

    public Node3D GetTopRandomNode(double percent = 0.1) => (Node3D)(GodotObject)refCounted.Call(MethodName.GetTopRandomNode, percent);

    public Vector3 GetTopRandomPosition(double percent = 0.1) => (Vector3)refCounted.Call(MethodName.GetTopRandomPosition, percent);

    public bool HasResult() => (bool)refCounted.Call(MethodName.HasResult);

    private static class MethodName
    {
        public static readonly StringName GetAllNode = "get_all_node";
        public static readonly StringName GetAllPosition = "get_all_position";
        public static readonly StringName GetAllResults = "get_all_results";
        public static readonly StringName GetHighestScoreNode = "get_highest_score_node";
        public static readonly StringName GetHighestScorePosition = "get_highest_score_position";
        public static readonly StringName GetTopRandomNode = "get_top_random_node";
        public static readonly StringName GetTopRandomPosition = "get_top_random_position";
        public static readonly StringName HasResult = "has_result";
    }
}
