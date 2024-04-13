bool Element::rareDataChildrenAffectedByHover() const
{
    ASSERT(hasRareData());
    return elementRareData()->childrenAffectedByHover();
}
