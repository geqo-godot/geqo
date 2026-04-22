using Godot;

public partial class EnvironmentQueryWrapper2D(Node node)
{
    private readonly Node node = node;
    /// <summary>
    /// Returns the raw node of EnvironmentQuery, in case it's methods are needed, like queue_free().
    /// </summary>
    public Node QueryNode => node;
    public Node2D Querier
    {
        get => (Node2D)(GodotObject)node.Call("get_querier");
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
        node.Call(Methods.RequestQuery);
    }

    public QueryResultWrapper2D GetResult()
    {
        return new QueryResultWrapper2D((RefCounted)(GodotObject)node.Call(Methods.GetResult));
    }

    public SignalAwaiter QueryFinished
        => node.ToSignal(node, Signals.QueryFinished);

    private static class Methods
    {
        public static readonly StringName GetResult = "get_result";
        public static readonly StringName RequestQuery = "request_query";
    }

    private static class Signals
    {
        public static readonly StringName QueryFinished = "query_finished";
    }
}
