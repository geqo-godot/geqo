using Godot;
public partial class QueryInstanceWrapper2D(RefCounted refCounted)
{
    private readonly RefCounted refCounted = refCounted;
    public RefCounted RawQueryInstance => refCounted;

    private QueryContextWrapper2D _querierContext;
    public QueryContextWrapper2D QuerierContext =>
        _querierContext ??= new QueryContextWrapper2D(
            (Node2D)(GodotObject)refCounted.Call("get_querier_context")
        );

    public void AddItem(QueryItemWrapper2D item)
    {
        refCounted.Call(MethodName.AddItem, item.RawQueryItem);
    }

    public QueryItemWrapper2D GetCurrentQueryItem()
    {
        return new QueryItemWrapper2D((RefCounted)(GodotObject)refCounted.Call(MethodName.GetCurrentQueryItem));
    }

    public QueryItemWrapper2D GetItem(int index)
    {
        return new QueryItemWrapper2D((RefCounted)(GodotObject)refCounted.Call(MethodName.GetItem, index));
    }

    public int GetItemCount()
    {
        return (int)refCounted.Call(MethodName.GetItemCount);
    }

    public QueryItemWrapper2D GetNextItem()
    {
        return new QueryItemWrapper2D((RefCounted)(GodotObject)refCounted.Call(MethodName.GetNextItem));
    }

    public float GetTestDataMax(GodotObject test)
    {
        return (float)refCounted.Call(MethodName.GetTestDataMax, test);
    }

    public float GetTestDataMin(GodotObject test)
    {
        return (float)refCounted.Call(MethodName.GetTestDataMin, test);
    }

    public bool HasItems()
    {
        return (bool)refCounted.Call(MethodName.HasItems);
    }

    public bool HasTestData(GodotObject test)
    {
        return (bool)refCounted.Call(MethodName.HasTestData, test);
    }

    public bool HasTimeLeft()
    {
        return (bool)refCounted.Call(MethodName.HasTimeLeft);
    }

    public void RefreshTimer()
    {
        refCounted.Call(MethodName.RefreshTimer);
    }

    public void ResetIterator()
    {
        refCounted.Call(MethodName.ResetIterator);
    }

    public void SetTestDataMax(GodotObject test, float max)
    {
        refCounted.Call(MethodName.SetTestDataMax, test, max);
    }

    public void SetTestDataMin(GodotObject test, float max)
    {
        refCounted.Call(MethodName.SetTestDataMin, test, max);
    }

    private static class MethodName
    {
        public static readonly StringName AddItem = "add_item";
        public static readonly StringName GetCurrentQueryItem = "get_current_query_item";
        public static readonly StringName GetItem = "get_item";
        public static readonly StringName GetItemCount = "get_item_count";
        public static readonly StringName GetNextItem = "get_next_item";
        public static readonly StringName GetTestDataMax = "get_test_data_max";
        public static readonly StringName GetTestDataMin = "get_test_data_min";
        public static readonly StringName HasItems = "has_items";
        public static readonly StringName HasTestData = "has_test_data";
        public static readonly StringName HasTimeLeft = "has_time_left";
        public static readonly StringName RefreshTimer = "refresh_timer";
        public static readonly StringName ResetIterator = "reset_iterator";
        public static readonly StringName SetTestDataMax = "set_test_data_max";
        public static readonly StringName SetTestDataMin = "set_test_data_min";
    }
}
