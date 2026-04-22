using Godot;
[Tool]
public partial class QueryContextWrapper3D(Node3D node)
{
	private readonly Node3D node = node;
    public Node3D RawQueryContext => node;
    public Godot.Collections.Array GetContext(QueryInstanceWrapper3D queryInstance)
	{
		return (Godot.Collections.Array)node.Call(Methods.GetContext, queryInstance.RawQueryInstance);
	}

	public Godot.Collections.Array<Node3D> GetContextNodes(QueryInstanceWrapper3D queryInstance)
	{
		return (Godot.Collections.Array<Node3D>)node.Call(Methods.GetContextNodes, queryInstance.RawQueryInstance);
	}

	public Vector3[] GetContextPositions(QueryInstanceWrapper3D queryInstance)
	{
		return (Vector3[])node.Call(Methods.GetContextPositions, queryInstance.RawQueryInstance);
	}
	private static class Methods
	{
		public static readonly StringName GetContext = "get_context";
		public static readonly StringName GetContextNodes = "get_context_nodes";
		public static readonly StringName GetContextPositions = "get_context_positions";
	}
}
