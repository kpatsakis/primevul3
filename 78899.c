bool Element::isInCanvasSubtree() const
{
    return hasRareData() && elementRareData()->isInCanvasSubtree();
}
