bool Element::isInTopLayer() const
{
    return hasRareData() && elementRareData()->isInTopLayer();
}
