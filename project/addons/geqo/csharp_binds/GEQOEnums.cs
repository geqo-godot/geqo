public static class GEQOEnums
{
    public enum TestPurpose
    {
        FilterScore = 0,
        FilterOnly = 1,
        ScoreOnly = 2
    }

    public enum TestType
    {
        Numeric = 0,
        Boolean = 1
    }

    public enum FilterType
    {
        Min = 0,
        Max = 1,
        Range = 2
    }

    public enum MultipleContextScoreOp
    {
        AverageScore = 0,
        MaxScore = 1,
        MinScore = 2
    }

    public enum MultipleContextFilterOp
    {
        AnyPass = 0,
        AllPass = 1
    }

    public enum ScoreClampType
    {
        None = 0,
        Val = 1,
        Filter = 2
    }
}