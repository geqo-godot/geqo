using Godot;
[Tool]
public partial class QueryContext3D : Node3D
{
	/// <summary>
	/// The GDExtension calls this function
	/// </summary>
	Godot.Collections.Array _get_context(RefCounted queryInstance)
	{
		QueryInstanceWrapper3D instance = new QueryInstanceWrapper3D(queryInstance);
		return _GetContext(instance);
	}

	public virtual Godot.Collections.Array _GetContext(QueryInstanceWrapper3D queryInstance)
	{
		GD.PrintErr("Need to override _GetContext in derived class");
		return [];
	}
}
