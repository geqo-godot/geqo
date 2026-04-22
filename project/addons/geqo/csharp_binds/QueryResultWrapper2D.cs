using Godot;
/// <summary>
/// Wraps over the GDExtension QueryResult2D.
/// </summary>
public partial class QueryResultWrapper2D(RefCounted refCounted)
{
    private readonly RefCounted refCounted = refCounted;

    public Godot.Collections.Array<Node2D> GetAllNode()
    {
        return (Godot.Collections.Array<Node2D>)refCounted.Call(Methods.GetAllNode);
    }

    public Vector2[] GetAllPosition()
    {
        return (Vector2[])refCounted.Call(Methods.GetAllPosition);
    }

    public Godot.Collections.Array<QueryItemWrapper2D> GetAllResults()
    {
        var result = new Godot.Collections.Array<QueryItemWrapper2D>();
        foreach (RefCounted item in (Godot.Collections.Array)refCounted.Call(Methods.GetAllResults))
            result.Add(new QueryItemWrapper2D(item));
        return result;
    }

    public Node2D GetHighestScoreNode()
    {
        return (Node2D)(GodotObject)refCounted.Call(Methods.GetHighestScoreNode);
    }

    public Vector2 GetHighestScorePosition()
    {
        return (Vector2)refCounted.Call(Methods.GetHighestScorePosition);
    }

    public Node2D GetTopRandomNode(double percent = 0.1)
    {
        return (Node2D)(GodotObject)refCounted.Call(Methods.GetTopRandomNode, percent);
    }

    public Vector2 GetTopRandomPosition(double percent = 0.1)
    {
        return (Vector2)refCounted.Call(Methods.GetTopRandomPosition, percent);
    }

    public bool HasResult()
    {
        return (bool)refCounted.Call(Methods.HasResult);
    }

    private static class Methods
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
