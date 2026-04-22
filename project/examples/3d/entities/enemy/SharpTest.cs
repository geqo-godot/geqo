using Godot;
[Tool]
public partial class SharpTest : QueryTest3D
{
	public override void _EnterTree()
	{
		Call("set_test_type", 1);
		base._EnterTree();
	}
	public override void _PerformTest(QueryInstanceWrapper3D queryInstance)
	{
		GD.Print("TEST");
		bool bool_match = (bool)Call("get_bool_match");
		GD.Print("Bool matched? ", bool_match);
		EndTest();
	}
}
