bool Element::hasPendingResources() const
{
    return hasRareData() && elementRareData()->hasPendingResources();
}
