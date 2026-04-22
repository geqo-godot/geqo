using Godot;
/// <summary>
/// Wraps over the GDExtension QueryResult3D.
/// </summary>
public partial class QueryResultWrapper3D(RefCounted refCounted)
{
	private readonly RefCounted refCounted = refCounted;

    public Godot.Collections.Array<Node3D> GetAllNode()
	{
		return (Godot.Collections.Array<Node3D>)refCounted.Call(MethodName.GetAllNode);
	}

	public Vector3[] GetAllPosition()
	{
		return (Vector3[])refCounted.Call(MethodName.GetAllPosition);
	}

    public Godot.Collections.Array<QueryItemWrapper3D> GetAllResults()
    {
        var result = new Godot.Collections.Array<QueryItemWrapper3D>();
        foreach (RefCounted item in (Godot.Collections.Array)refCounted.Call(MethodName.GetAllResults))
            result.Add(new QueryItemWrapper3D(item));
        return result;
    }

    public Node3D GetHighestScoreNode()
	{
		return (Node3D)(GodotObject)refCounted.Call(MethodName.GetHighestScoreNode);
	}

	public Vector3 GetHighestScorePosition()
	{
		return (Vector3)refCounted.Call(MethodName.GetHighestScorePosition);
	}

	public Node3D GetTopRandomNode(double percent = 0.1)
	{
		return (Node3D)(GodotObject)refCounted.Call(MethodName.GetTopRandomNode, percent);
	}

	public Vector3 GetTopRandomPosition(double percent = 0.1)
	{
		return (Vector3)refCounted.Call(MethodName.GetTopRandomPosition, percent);
	}

	public bool HasResult()
	{
		return (bool)refCounted.Call(MethodName.HasResult);
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
