void Element::setChildrenAffectedByDrag(bool value)
{
    if (value || hasRareData())
        ensureElementRareData()->setChildrenAffectedByDrag(value);
}
