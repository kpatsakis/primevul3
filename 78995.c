ElementShadow* Element::shadow() const
{
    return hasRareData() ? elementRareData()->shadow() : 0;
}
