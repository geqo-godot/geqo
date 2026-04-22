using Godot;
[Tool]
public partial class QueryContext2D : Node2D
{
    /// <summary>
    /// The GDExtension calls this function
    /// </summary>
    Godot.Collections.Array _get_context(RefCounted queryInstance)
    {
        QueryInstanceWrapper2D instance = new QueryInstanceWrapper2D(queryInstance);
        return _GetContext(instance);
    }

    public virtual Godot.Collections.Array _GetContext(QueryInstanceWrapper2D queryInstance)
    {
        GD.PrintErr("Need to override _GetContext in derived class");
        return [];
    }

    private static class Methods
    {
        public static readonly StringName GetContext = "get_context";
        public static readonly StringName GetContextNodes = "get_context_nodes";
        public static readonly StringName GetContextPositions = "get_context_positions";
    }
}

