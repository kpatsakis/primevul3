void Element::setContainsFullScreenElement(bool flag)
{
    ensureElementRareData()->setContainsFullScreenElement(flag);
    setNeedsStyleRecalc(SyntheticStyleChange);
}
