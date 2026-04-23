using Godot;
using Godot.Collections;
[Tool]
public partial class QueryContext2D : Node2D
{
    /// <summary>
    /// The GDExtension calls this function
    /// </summary>
    private Array _get_context(RefCounted queryInstance)
    {
        QueryInstanceWrapper2D instance = new QueryInstanceWrapper2D(queryInstance);
        return _GetContext(instance);
    }

    public virtual Array _GetContext(QueryInstanceWrapper2D queryInstance)
    {
        GD.PrintErr("Need to override _GetContext in derived class");
        return [];
    }
}
