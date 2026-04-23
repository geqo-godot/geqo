using Godot;
[Tool]
public partial class SharpContext : QueryContext3D
{
	[Export]
	public Node3D targetNode;
	public override Godot.Collections.Array _GetContext(QueryInstanceWrapper3D queryInstance)
	{
		return [targetNode];
	}
}
