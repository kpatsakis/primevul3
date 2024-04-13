bool Element::hasFlagsSetDuringStylingOfChildren() const
{
    if (!hasRareData())
        return false;
    return rareDataChildrenAffectedByHover()
        || rareDataChildrenAffectedByActive()
        || rareDataChildrenAffectedByDrag()
        || rareDataChildrenAffectedByFirstChildRules()
        || rareDataChildrenAffectedByLastChildRules()
        || rareDataChildrenAffectedByDirectAdjacentRules()
        || rareDataChildrenAffectedByForwardPositionalRules()
        || rareDataChildrenAffectedByBackwardPositionalRules();
}
