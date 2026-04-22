using Godot;

public partial class EnvironmentQueryWrapper3D(Node node)
{
	private readonly Node node = node;
	/// <summary>
	/// Returns the raw node of EnvironmentQuery, in case it's methods are needed, like queue_free().
	/// </summary>
	public Node QueryNode => node;
    public Node3D Querier
	{
		get => (Node3D)(GodotObject)node.Call("get_querier");
		set => node.Call("set_querier", value);
	}
	public float TimeBudgetMs
	{
		get => (float)node.Call("get_time_budget_ms");
		set => node.Call("set_time_budget_ms", value);
	}
	public float UseDebugShapes
	{
		get => (float)node.Call("get_use_debug_shapes");
		set => node.Call("set_use_debug_shapes", value);
	}

	public void RequestQuery()
	{
		node.Call(MethodName.RequestQuery);
	}

	public QueryResultWrapper3D GetResult()
	{
		return new QueryResultWrapper3D((RefCounted)(GodotObject)node.Call(MethodName.GetResult));
	}

	public SignalAwaiter QueryFinished
		=>node.ToSignal(node, SignalName.QueryFinished);

	private static class MethodName
	{
		public static readonly StringName GetResult = "get_result";
		public static readonly StringName RequestQuery = "request_query";
	}

	private static class SignalName
	{
		public static readonly StringName QueryFinished = "query_finished";
	}
}
