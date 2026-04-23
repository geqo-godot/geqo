using Godot;
/// <summary>
/// Wraps over the GDExtension QueryResult2D.
/// </summary>
using Godot.Collections;
public partial class QueryResultWrapper2D(RefCounted refCounted)
{
    private readonly RefCounted refCounted = refCounted;

    public Array<Node2D> GetAllNode() => (Array<Node2D>)refCounted.Call(MethodName.GetAllNode);

    public Vector2[] GetAllPosition() => (Vector2[])refCounted.Call(MethodName.GetAllPosition);

    public Array<QueryItemWrapper2D> GetAllResults()
    {
        var result = new Array<QueryItemWrapper2D>();
        foreach (RefCounted item in (Array)refCounted.Call(MethodName.GetAllResults))
            result.Add(new QueryItemWrapper2D(item));
        return result;
    }

    public Node2D GetHighestScoreNode() => (Node2D)(GodotObject)refCounted.Call(MethodName.GetHighestScoreNode);

    public Vector2 GetHighestScorePosition() => (Vector2)refCounted.Call(MethodName.GetHighestScorePosition);

    public Node2D GetTopRandomNode(double percent = 0.1) => (Node2D)(GodotObject)refCounted.Call(MethodName.GetTopRandomNode, percent);

    public Vector2 GetTopRandomPosition(double percent = 0.1) => (Vector2)refCounted.Call(MethodName.GetTopRandomPosition, percent);

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
