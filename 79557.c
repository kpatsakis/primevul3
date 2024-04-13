static bool requiresSquashing(CompositingReasons reasons)
{
    return !requiresCompositing(reasons) && (reasons & CompositingReasonComboSquashableReasons);
}
