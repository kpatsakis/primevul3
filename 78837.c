void Element::clearHasPendingResources()
{
    ensureElementRareData()->setHasPendingResources(false);
}
