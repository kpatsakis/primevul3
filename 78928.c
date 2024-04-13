RenderObject* Element::pseudoElementRenderer(PseudoId pseudoId) const
{
    if (PseudoElement* element = pseudoElement(pseudoId))
        return element->renderer();
    return 0;
}
