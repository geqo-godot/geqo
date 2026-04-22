using Godot;
[Tool]
public partial class SharpTest : QueryTest3D
{
	[Export]
	public Node3D context;
	/// <summary>
	/// This wrapper is necessary because C# doesn't recognize QueryContext3D from GDExtension as the same as C# QueryContext3D.
	/// </summary>
	public QueryContextWrapper3D Context;
	public override void _EnterTree()
	{
		Context = new QueryContextWrapper3D(context);
		Call("set_test_type", 1);
		base._EnterTree();
	}
	public override void _PerformTest(QueryInstanceWrapper3D queryInstance)
	{
		GD.Print("TEST");
		bool bool_match = (bool)Call("get_bool_match");
		GD.Print("Bool matched? ", bool_match);
		GD.Print("Context length: ", Context.GetContextPositions(queryInstance).Length);
		EndTest();
	}
}
