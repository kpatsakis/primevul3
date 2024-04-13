inline ElementRareData* Element::elementRareData() const
{
    ASSERT(hasRareData());
    return static_cast<ElementRareData*>(rareData());
}
