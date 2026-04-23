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
        get => (Node3D)(GodotObject)node.Call(MethodName.GetQuerier);
        set => node.Call(MethodName.SetQuerier, value);
    }
    public float TimeBudgetMs
    {
        get => (float)node.Call(MethodName.GetTimeBudgetMs);
        set => node.Call(MethodName.SetBudgetTimeMs, value);
    }
    public float UseDebugShapes
    {
        get => (float)node.Call(MethodName.GetUseDebugShapes);
        set => node.Call(MethodName.SetUseDebugShapes, value);
    }

    public void RequestQuery() => node.Call(MethodName.RequestQuery);

    public QueryResultWrapper3D GetResult() => new QueryResultWrapper3D((RefCounted)(GodotObject)node.Call(MethodName.GetResult));

    public SignalAwaiter QueryFinished
        => node.ToSignal(node, SignalName.QueryFinished);

    private static class MethodName
    {
        public static readonly StringName GetResult = "get_result";
        public static readonly StringName RequestQuery = "request_query";
	public static readonly StringName GetQuerier = "get_querier";
	public static readonly StringName SetQuerier = "set_querier";
	public static readonly StringName GetTimeBudgetMs = "get_time_budget_ms";
	public static readonly StringName SetBudgetTimeMs = "set_time_budget_ms";
	public static readonly StringName GetUseDebugShapes = "get_use_debug_shapes";
	public static readonly StringName SetUseDebugShapes = "set_use_debug_shapes";
    }

    private static class SignalName
    {
        public static readonly StringName QueryFinished = "query_finished";
    }
}
