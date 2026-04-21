using Godot;

public partial class QueryResult3D
{
	private readonly RefCounted result;

	public QueryResult3D(RefCounted refCounted)
	{
		result = refCounted;
	}

	public Godot.Collections.Array<Node3D> GetAllNode()
	{
		return (Godot.Collections.Array<Node3D>)result.Call(MethodName.GetAllNode);
	}

	public Vector3[] GetAllPosition()
	{
		return (Vector3[])result.Call(MethodName.GetAllNode);

	}

	public Godot.Collections.Array<QueryItem3D> GetAllResults()
	{
		Godot.Collections.Array<QueryItem3D> items = [];
		foreach (RefCounted refItem in (Godot.Collections.Array)result.Call(MethodName.GetAllResults))
			items.Add(new QueryItem3D(refItem));
		return items;
	}

	public Node3D GetHighestScoreNode()
	{
		return (Node3D)result.Call(MethodName.GetHighestScoreNode);
	}

	public Vector3 GetHighestScorePosition()
	{
		return (Vector3)result.Call(MethodName.GetHighestScorePosition);
	}

	public Node3D GetTopRandomNode(double percent = 0.1)
	{
		return (Node3D)result.Call(MethodName.GetTopRandomNode, percent);
	}

	public Vector3 GetTopRandomPosition(double percent = 0.1)
	{
		return (Vector3)result.Call(MethodName.GetTopRandomPosition, percent);
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
