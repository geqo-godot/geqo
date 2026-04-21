using Godot;

public class EnvironmentQuery3D
{
	public Node3D querier
	{
		get => (Node3D)envQuery.Call("get_querier");
		set => envQuery.Call("set_querier", value);
	}
    public float timeBudgetMs
    {
        get => (float)envQuery.Call("get_time_budget_ms");
        set => envQuery.Call("set_time_budget_ms", value);
    }
    public float useDebugShapes
    {
        get => (float)envQuery.Call("get_use_debug_shapes");
        set => envQuery.Call("set_use_debug_shapes", value);
    }
    private readonly Node envQuery;


	public EnvironmentQuery3D(Node node)
	{
		envQuery = node;
	}

	public void RequestQuery()
	{
		envQuery.Call(MethodName.RequestQuery);
	}

	public QueryResult3D GetResult()
	{
		return new QueryResult3D((RefCounted)envQuery.Call(MethodName.GetResult));
	}

	public SignalAwaiter QueryFinished
		=> envQuery.ToSignal(envQuery, SignalName.QueryFinished);

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
