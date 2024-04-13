PseudoElement* Element::pseudoElement(PseudoId pseudoId) const
{
    return hasRareData() ? elementRareData()->pseudoElement(pseudoId) : 0;
}
