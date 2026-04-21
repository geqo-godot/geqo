using Godot;

public class EnvironmentQuery3D
{
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
