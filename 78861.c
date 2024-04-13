void Element::didAffectSelector(AffectedSelectorMask mask)
{
    setNeedsStyleRecalc();
    if (ElementShadow* elementShadow = shadowOfParentForDistribution(this))
        elementShadow->didAffectSelector(mask);
}
