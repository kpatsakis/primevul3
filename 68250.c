DetectRunTxSortHelper(const void *a, const void *b)
{
    const RuleMatchCandidateTx *s0 = a;
    const RuleMatchCandidateTx *s1 = b;
    if (s1->id == s0->id)
        return 0;
    else
        return s0->id > s1->id ? -1 : 1;
}
