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
	private EnvironmentQuery3D envQuery;
	public override void _Ready()
	{
		navAgent = GetNode<NavigationAgent3D>("NavigationAgent3D");
		envQuery = new EnvironmentQuery3D(GetNode("EnvironmentQuery3D"));
	}
	public override async void _Input(InputEvent @event)
	{
		if (@event.IsActionPressed("request_query"))
		{
			envQuery.RequestQuery();
			await envQuery.QueryFinished;
			QueryResult3D queryResult = envQuery.GetResult();
			if (!queryResult.HasResult())
				return;
			GD.Print("Calling all result functions");
			finalTarget = queryResult.GetHighestScorePosition();
			queryResult.GetTopRandomPosition();
			Godot.Collections.Array<QueryItem3D> itemResults = queryResult.GetAllResults();
			queryResult.GetAllNode();
			queryResult.GetAllPosition();
			queryResult.GetHighestScoreNode();
			queryResult.GetTopRandomNode();
			queryResult.GetTopRandomPosition();
			navAgent.TargetPosition = finalTarget;
			currentTarget = navAgent.GetNextPathPosition();

			QueryItem3D firstItem = itemResults[0];
			GD.Print("Collided with: ", firstItem.collidedWith);
			GD.Print("Is filtered: ", firstItem.isFiltered);
			GD.Print("Projection position: ", firstItem.projectionPosition);
			GD.Print("Score: ", firstItem.score);
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
