using Godot;
using System.Threading.Tasks;

public partial class SharpEnemy : CharacterBody3D
{
	private const float SPEED = 500f;
	private enum State { Idle, Walking }
	private State currentState = State.Idle;
	private Vector3 finalTarget;
	private Vector3? currentTarget = null;
	private NavigationAgent3D navAgent;
	private Node envQuery;
	public override void _Ready()
	{
		navAgent = GetNode<NavigationAgent3D>("NavigationAgent3D");
		envQuery = GetNode("EnvironmentQuery3D");
	}
	public override async void _Input(InputEvent @event)
	{
		if (@event.IsActionPressed("request_query"))
		{
			envQuery.Call("request_query");
			await ToSignal(envQuery, "query_finished");
			GodotObject queryResult = (GodotObject)envQuery.Call("get_result");
			bool hasResult = (bool)queryResult.Call("has_result");
			if (!hasResult)
				return;
			finalTarget = (Vector3)queryResult.Call("get_highest_score_position");
			navAgent.TargetPosition = finalTarget;
			currentTarget = navAgent.GetNextPathPosition();
		}
	}
	public override void _PhysicsProcess(double delta)
	{
		float d = (float)delta;
		if (!IsOnFloor())
			Velocity += GetGravity() * d;
		switch (currentState)
		{
			case State.Idle:
				Idle();
				break;
			case State.Walking:
				Walking(d);
				break;
		}
		MoveAndSlide();
	}
	private void MoveToTarget(float delta, Vector3 target)
	{
		Vector3 direction = GlobalPosition.DirectionTo(target);
		Velocity = new Vector3(
			direction.X * SPEED * delta,
			Velocity.Y,
			direction.Z * SPEED * delta
		);
	}
	private void Idle()
	{
		Velocity = new Vector3(
			Mathf.Lerp(Velocity.X, 0f, 0.25f),
			Velocity.Y,
			Mathf.Lerp(Velocity.Z, 0f, 0.25f)
		);
		if (currentTarget != null)
			currentState = State.Walking;
	}
	private void Walking(float delta)
	{
		if (currentTarget == null)
		{
			currentState = State.Idle;
			return;
		}
		currentTarget = navAgent.GetNextPathPosition();
		MoveToTarget(delta, currentTarget.Value);
	}
	private void _OnNavigationAgent3DTargetReached()
	{
		currentTarget = null;
	}
}
