using Godot;
using Godot.Collections;
using Arr = Godot.Collections.Array;
public partial class QueryContextWrapper2D(Node2D node)
{
    private readonly Node2D node = node;
    public Node2D RawQueryContext => node;
    public Arr GetContext(QueryInstanceWrapper2D queryInstance) => (Arr)node.Call(Methods.GetContext, queryInstance.RawQueryInstance);

    public Array<Node2D> GetContextNodes(QueryInstanceWrapper2D queryInstance) => (Array<Node2D>)node.Call(Methods.GetContextNodes, queryInstance.RawQueryInstance);

    public Vector2[] GetContextPositions(QueryInstanceWrapper2D queryInstance) => (Vector2[])node.Call(Methods.GetContextPositions, queryInstance.RawQueryInstance);
    private static class Methods
    {
        public static readonly StringName GetContext = "get_context";
        public static readonly StringName GetContextNodes = "get_context_nodes";
        public static readonly StringName GetContextPositions = "get_context_positions";
    }
}
