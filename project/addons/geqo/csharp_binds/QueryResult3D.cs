using Godot;
using System;

public partial class QueryResult3D
{
	private readonly RefCounted result;

	public QueryResult3D(RefCounted refCounted)
	{
		result = refCounted;
	}
	
	public Vector3 GetHighestScorePosition()
	{
		return (Vector3)result.Call(MethodName.GetHighestScorePosition);
	}

	public bool HasResult()
	{
		return (bool)result.Call(MethodName.HasResult);
	}

	private static class MethodName
	{
		public static readonly StringName GetAllNode = "get_all_node";
		public static readonly StringName GetAllPosition = "get_all_position";
		public static readonly StringName GetHighestScoreNode = "get_highest_score_node";
		public static readonly StringName GetHighestScorePosition = "get_highest_score_position";
		public static readonly StringName GetTopRandomNode = "get_top_random_node";
		public static readonly StringName GetTopRandomPosition = "get_top_random_position";
		public static readonly StringName HasResult = "has_result";
	}
}
