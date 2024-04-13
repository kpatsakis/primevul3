inline ElementRareData* Element::ensureElementRareData()
{
    return static_cast<ElementRareData*>(ensureRareData());
}
