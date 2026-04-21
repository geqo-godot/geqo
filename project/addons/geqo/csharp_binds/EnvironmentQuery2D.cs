using Godot;

public class EnvironmentQuery2D
{
    private readonly Node envQuery;
    public EnvironmentQuery2D(Node node)
    {
        envQuery = node;
    }

    public void RequestQuery()
    {
        envQuery.Call(MethodName.RequestQuery);
    }

    public QueryResult2D GetResult()
    {
        return new QueryResult2D((RefCounted)envQuery.Call(MethodName.GetResult));
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
