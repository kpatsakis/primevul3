bool Element::rareDataChildrenAffectedByActive() const
{
    ASSERT(hasRareData());
    return elementRareData()->childrenAffectedByActive();
}
