static bool requiresCompositing(CompositingReasons reasons)
{
    return reasons & ~CompositingReasonComboSquashableReasons;
}
