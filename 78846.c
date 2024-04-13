RenderStyle* Element::computedStyle(PseudoId pseudoElementSpecifier)
{
    if (PseudoElement* element = pseudoElement(pseudoElementSpecifier))
        return element->computedStyle();

    if (RenderStyle* usedStyle = renderStyle()) {
        if (pseudoElementSpecifier) {
            RenderStyle* cachedPseudoStyle = usedStyle->getCachedPseudoStyle(pseudoElementSpecifier);
            return cachedPseudoStyle ? cachedPseudoStyle : usedStyle;
         } else
            return usedStyle;
    }

    if (!attached())
        return 0;

    ElementRareData* data = ensureElementRareData();
    if (!data->computedStyle())
        data->setComputedStyle(document()->styleForElementIgnoringPendingStylesheets(this));
    return pseudoElementSpecifier ? data->computedStyle()->getCachedPseudoStyle(pseudoElementSpecifier) : data->computedStyle();
}
