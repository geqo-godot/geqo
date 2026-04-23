using Godot;
using Godot.Collections;
using Arr = Godot.Collections.Array;
public partial class QueryGenerator3D : Node3D
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
        QueryInstanceWrapper3D instance = new QueryInstanceWrapper3D(queryInstance);
        _PerformGeneration(instance);
    }

    public virtual void _PerformGeneration(QueryInstanceWrapper3D queryInstance)
    {
        GD.PrintErr("Need to override PerformTest in derived class");
    }

    public Dictionary CastRayProjection(Vector3 startPos, Vector3 endPos, Arr exclusions, int colMask) => (Dictionary)Call(Methods.CastRayProjection, startPos, endPos, exclusions, colMask);

    public Array<Dictionary> CastShapeProjection(Vector3 startPos, Vector3 endPos, Arr exclusions, Shape3D shape, int colMask) => (Array<Dictionary>)Call(Methods.CastRayProjection, startPos, endPos, exclusions, shape, colMask);

    private static class Methods
    {
        public static readonly StringName CastRayProjection = "cast_ray_projection";
        public static readonly StringName CastShapeProjection = "cast_shape_projection";
	public static readonly StringName GetRaycastMode = "get_raycast_mode";
	public static readonly StringName SetRaycastMode = "set_raycast_mode";
    }
}
