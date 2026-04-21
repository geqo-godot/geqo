using Godot;

public partial class QueryResult2D
{
    private readonly RefCounted result;

    public QueryResult2D(RefCounted refCounted)
    {
        result = refCounted;
    }

    public Godot.Collections.Array<Node2D> GetAllNode()
    {
        return (Godot.Collections.Array<Node2D>)result.Call(MethodName.GetAllNode);
    }

    public Vector2[] GetAllPosition()
    {
        return (Vector2[])result.Call(MethodName.GetAllPosition);

    }
    public Godot.Collections.Array<QueryItem3D> GetAllResults()
    {
        Godot.Collections.Array<QueryItem3D> items = [];
        foreach (RefCounted refItem in (Godot.Collections.Array)result.Call(MethodName.GetAllResults))
            items.Add(new QueryItem3D(refItem));
        return items;
    }
    public Node2D GetHighestScoreNode()
    {
        return (Node2D)result.Call(MethodName.GetHighestScoreNode);
    }

    public Vector2 GetHighestScorePosition()
    {
        return (Vector2)result.Call(MethodName.GetHighestScorePosition);
    }

    public Node2D GetTopRandomNode(double percent = 0.1)
    {
        return (Node2D)result.Call(MethodName.GetTopRandomNode, percent);
    }

    public Vector2 GetTopRandomPosition(double percent = 0.1)
    {
        return (Vector2)result.Call(MethodName.GetTopRandomPosition, percent);
    }

    public bool HasResult()
    {
        return (bool)result.Call(MethodName.HasResult);
    }

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
