void Element::setIsInTopLayer(bool inTopLayer)
{
    if (isInTopLayer() == inTopLayer)
        return;
    ensureElementRareData()->setIsInTopLayer(inTopLayer);

    lazyReattachIfAttached();
}
