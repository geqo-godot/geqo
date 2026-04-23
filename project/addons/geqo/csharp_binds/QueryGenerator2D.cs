using Godot;
using Godot.Collections;
using Arr = Godot.Collections.Array;
public partial class QueryGenerator2D : Node2D
{
    public enum RaycastModeEnum
    {
        Body,
        Area,
        BodyArea,
    }

    public RaycastModeEnum RaycastMode
    {
        get => (RaycastModeEnum)(int)Call(Methods.GetRaycastMode);
        set => Call(Methods.SetRaycastMode, (int)value);
    }
    /// <summary>
    /// The GDExtension calls this function.
    /// </summary>
    private void _perform_generation(RefCounted queryInstance)
    {
        QueryInstanceWrapper2D instance = new QueryInstanceWrapper2D(queryInstance);
        _PerformGeneration(instance);
    }

    public virtual void _PerformGeneration(QueryInstanceWrapper2D queryInstance)
    {
        GD.PrintErr("Need to override PerformTest in derived class");
    }

    public Dictionary CastRayProjection(Vector2 startPos, Vector2 endPos, Arr exclusions, int colMask) => (Dictionary)Call(Methods.CastRayProjection, startPos, endPos, exclusions, colMask);

    public Array<Dictionary> CastShapeProjection(Vector2 startPos, Vector2 endPos, Arr exclusions, Shape2D shape, int colMask) => (Array<Dictionary>)Call(Methods.CastRayProjection, startPos, endPos, exclusions, shape, colMask);

    private static class Methods
    {
        public static readonly StringName CastRayProjection = "cast_ray_projection";
        public static readonly StringName CastShapeProjection = "cast_shape_projection";
	public static readonly StringName GetRaycastMode = "get_raycast_mode";
	public static readonly StringName SetRaycastMode = "set_raycast_mode";
    }
}
