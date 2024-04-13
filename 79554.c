static bool requiresCompositingOrSquashing(CompositingReasons reasons)
{
#ifndef NDEBUG
    bool fastAnswer = reasons != CompositingReasonNone;
    bool slowAnswer = requiresCompositing(reasons) || requiresSquashing(reasons);
    ASSERT(fastAnswer == slowAnswer);
#endif
    return reasons != CompositingReasonNone;
}
