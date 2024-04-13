ElementShadow* Element::ensureShadow()
{
    return ensureElementRareData()->ensureShadow();
}
